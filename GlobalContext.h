#ifndef _GLOBALCONTEXT_H_
#define _GLOBALCONTEXT_H_

#include <cstdio>
#include <pthread.h>
#include <vector>
#include <algorithm>
#include <atomic>
#include <iostream>
#include <map>
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
 public:
  const MapReduceClient &client;
  const InputVec &input_vec;
  OutputVec &output_vec;
  pthread_mutex_t output_vec_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t wait_for_job_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t get_job_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t map_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t reduce_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
  MidVectors shuffled_vectors;
  std::map<K2*, std::vector<IntermediatePair>*> shuffle_map;
  Barrier *threads_barrier;
  bool is_job_ended = false;
  int intermediary_elements;

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
           / (float) intermediary_elements * 100.0;
  }
  int get_pairs_number () const
  { return this->pairs_number; }

  // Atmoic variables
  uint32_t increment_next_pair_index ()
  { return (this->next_pair_index)++; }
  void set_stage_and_reset_general_atomic (stage_t stage);
  void increment_first_counter_general_atomic (uint64_t inc = 1);
  void increment_second_counter_general_atomic (uint64_t inc = 1);
  stage_t get_stage ();
  uint64_t get_first_counter_value ();
  uint64_t get_second_counter_value ();
  void reset_next_pair_index ()
  { next_pair_index.exchange (0); }
  ~GlobalContext ();
};

#endif //_GLOBALCONTEXT_H_
