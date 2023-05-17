#include "MapReduceFramework.h"
#include "ThreadContext.h"
#include "MapReduce.h"
#include "GlobalContext.h"
#include <utility>

JobHandle
startMapReduceJob (const MapReduceClient &client, const InputVec &inputVec,
                   OutputVec &outputVec, int multiThreadLevel)
{
  auto *global_context = new GlobalContext (
      client, inputVec, outputVec, multiThreadLevel);
  MapReduce::start_job (multiThreadLevel, global_context);
  return global_context;
}

void emit2 (K2 *key, V2 *value, void *context)
{
  auto *tc = (ThreadContext *) context;
  GlobalContext *gc = tc->global_context;
  tc->append_to_map_vector (std::make_pair (key, value));
  gc->increment_intermediary_elements_number ();
}

void getJobState (JobHandle job, JobState *state)
{
  auto *global_context = (GlobalContext *) job;
  state->stage = global_context->get_stage ();
  switch (state->stage)
    {
      case UNDEFINED_STAGE:
        break;

      case SHUFFLE_STAGE:
      case REDUCE_STAGE:
        state->percentage = global_context->get_shuffle_progress_percentage ();
      break;

      case MAP_STAGE:
        state->percentage = global_context->get_map_progress_percentage ();
      break;

      default:
        break;
    }
}

void closeJobHandle (JobHandle job)
{
  waitForJob (job);
  auto *global_context = (GlobalContext *) job;
  MapReduce::free_system ();
  delete global_context;
}

void waitForJob (JobHandle job)
{
  auto *global_context = (GlobalContext *) job;
  pthread_mutex_lock (&global_context->wait_for_job_mutex);
  if (!global_context->is_job_ended)
    {
      for (auto it: MapReduce::threads)
        {
          if (pthread_join (*it, nullptr) != SUCCESS_CODE)
            {
              std::cout << PTHREAD_JOIN_FAILED;
              exit (EXIT_ERROR_CODE);
            }
        }
      global_context->is_job_ended = true;
    }
  pthread_mutex_unlock (&global_context->wait_for_job_mutex);
}

void emit3 (K3 *key, V3 *value, void *context)
{
  auto *tc = (ThreadContext *) context;
  GlobalContext *gc = tc->global_context;
  pthread_mutex_lock (&gc->output_vec_mutex);
  gc->output_vec.emplace_back (key, value);
  pthread_mutex_unlock (&gc->output_vec_mutex);
  gc->increment_first_counter_general_atomic (tc->curr_reduce_vector_size);
}