#ifndef _GLOBALCONTEXT_H_
#define _GLOBALCONTEXT_H_

#include <cstdio>
#include <pthread.h>
#include <vector>
#include <algorithm>
#include <atomic>
#include <iostream>
#include "MapReduceClient.h"
#include "MapReduceFramework.h"
#include "Barrier.h"

typedef std::vector<std::vector<IntermediatePair> *> MidVectors;

class ThreadContext;

class GlobalContext {
 private:
  std::atomic<uint32_t> next_pair_index{0};
  std::atomic<uint64_t> general_atomic{0};
  int pairs_number;
  stage_t stage;
 public:
  const MapReduceClient &client;
  const InputVec &input_vec;
  OutputVec &output_vec;
  int multi_thread_level;
  pthread_mutex_t output_vec_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t wait_for_job_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
  MidVectors shuffled_vectors;
  Barrier *threads_barrier;
  bool is_job_ended = false;

  GlobalContext (const MapReduceClient &client, const InputVec &input_vec,
                 OutputVec &output_vec, int multiThreadLevel);

  // Getters
  float get_map_progress_percentage ()
  {
    return (float) this->get_first_counter_value ()
           / (float) this->pairs_number * 100.0;
  }
  float get_shuffle_progress_percentage ()
  {
    return (float) this->get_first_counter_value ()
           / (float) get_second_counter_value () * 100.0;
  }
  int get_pairs_number () const
  { return this->pairs_number; }

  // Setters
  void set_stage (stage_t new_stage)
  { this->stage = new_stage; }

  // Atmoic variables
  uint32_t increment_next_pair_index ()
  { return (this->next_pair_index)++; }
  uint32_t increment_progress_counter (int inc = 1)
  { return this->progress_counter.fetch_add (inc); }
  uint32_t increment_intermediary_elements_number ()
  { return (this->intermediary_elements_number)++; }
  void reset_counters ();

  ~GlobalContext ();
};

#endif //_GLOBALCONTEXT_H_
