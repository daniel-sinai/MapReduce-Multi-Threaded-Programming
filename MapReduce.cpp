#include "MapReduce.h"

std::vector<pthread_t *> MapReduce::threads;
std::vector<ThreadContext *> MapReduce::contexts;

void MapReduce::start_job (int multi_thread_level, GlobalContext *global_context)
{
  for (int i = 0; i < multi_thread_level; ++i)
    {
      ThreadContext *curr_context = new ThreadContext (i, global_context);
      MapReduce::contexts.push_back (curr_context);
      pthread_t *curr_thread = new pthread_t ();
      if (pthread_create (curr_thread, NULL, job_manager, curr_context) != SUCCESS_CODE)
        {
          std::cout << PTHREAD_CREATE_FAILED;
          exit (EXIT_ERROR_CODE);
        }
      threads.push_back (curr_thread);
    }
}

void *MapReduce::job_manager (void *arg)
{
  ThreadContext *tc = (ThreadContext *) arg;
  GlobalContext *gc = tc->global_context;
  gc->set_stage (MAP_STAGE);
  map_manager (tc, gc);
  sort_mid_vector (tc);
  gc->threads_barrier->barrier ();
  if (tc->get_thread_id () == 0)
    {
      gc->reset_counters ();
      gc->set_stage (SHUFFLE_STAGE);
      shuffle_manager (tc, gc);
      gc->reset_counters ();
      gc->set_stage (REDUCE_STAGE);
    }
  gc->threads_barrier->barrier ();
  reduce_manager (tc, gc);
  gc->threads_barrier->barrier ();
  return nullptr;
}

void MapReduce::map_manager (ThreadContext *tc, GlobalContext *gc)
{
  uint32_t curr_id = gc->increment_next_pair_index ();
  while (curr_id < gc->get_pairs_number ())
    {
      InputPair key_value = gc->input_vec[curr_id];
      gc->client.map (key_value.first, key_value.second, tc);
      curr_id = gc->increment_next_pair_index ();
      gc->increment_progress_counter ();
    }
}

void MapReduce::sort_mid_vector (ThreadContext *tc)
{
  std::sort (tc->map_vector.begin (), tc->map_vector.end (), MapReduce::compare_k2);
}

void MapReduce::shuffle_manager (ThreadContext *tc, GlobalContext *gc)
{
  K2 *curr_max_k2 = find_max_k2_from_threads_vectors ();
  while (curr_max_k2 != nullptr)
    {
      std::vector<IntermediatePair> *curr_key_vector = new std::vector<IntermediatePair> ();
      for (auto curr_tc: contexts)
        {
          std::vector<IntermediatePair> &curr_thread_vector = curr_tc->map_vector;
          if (curr_thread_vector.empty ())
            { continue; }
          while (not curr_thread_vector.empty ()
                 and is_intermediary_keys_equal (curr_thread_vector.back ().first, curr_max_k2))
            {
              curr_key_vector->push_back (curr_thread_vector.back ());
              curr_thread_vector.pop_back ();
              gc->increment_progress_counter ();
            }
        }
      gc->shuffled_vectors.push_back (curr_key_vector);
      curr_max_k2 = find_max_k2_from_threads_vectors ();
    }
}

void MapReduce::reduce_manager (ThreadContext *tc, GlobalContext *gc)
{
  uint32_t curr_id = gc->increment_next_pair_index ();
  while (curr_id < gc->shuffled_vectors.size ())
    {
      IntermediateVec *mid_vector = gc->shuffled_vectors[curr_id];
      tc->curr_reduce_vector_size = mid_vector->size ();
      gc->client.reduce (mid_vector, tc);
      curr_id = gc->increment_next_pair_index ();
    }
}

K2 *MapReduce::find_max_k2_from_threads_vectors ()
{
  K2 *cur_max = nullptr;
  for (auto tc: MapReduce::contexts)
    {
      if (tc->map_vector.empty ())
        { continue; }
      K2 *cur_k2 = tc->map_vector.back ().first;
      if (cur_max == nullptr)
        {
          cur_max = cur_k2;
        }
      else if (*cur_max < *cur_k2)
        {
          cur_max = cur_k2;
        }
    }
  return cur_max;
}

bool MapReduce::is_intermediary_keys_equal (K2 *key1, K2 *key2)
{
  return (not(*key1 < *key2)) and (not(*key2 < *key1));
}

bool MapReduce::compare_k2 (const IntermediatePair &p1, const IntermediatePair &p2)
{
  return *(p1.first) < *(p2.first);
}
void MapReduce::free_system ()
{
  for (auto it: threads)
    {
      pthread_t *to_free = *it;
      delete to_free;
    }
  for (auto it: contexts)
    {
      ThreadContext *to_free = *it;
      delete to_free;
    }
}

