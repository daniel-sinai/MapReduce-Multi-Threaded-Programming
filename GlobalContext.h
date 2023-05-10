#ifndef _GLOBALCONTEXT_H_
#define _GLOBALCONTEXT_H_

#include <pthread.h>
#include <cstdio>
#include <pthread.h>
#include <vector>
#include <algorithm>
#include <atomic>
#include "MapReduceClient.h"
#include "MapReduceFramework.h"
#include "utils.h"
#include "Barrier.h"

//typedef void (*map_function) (const K1*, const V1*, void*);
//typedef void (*reduce_function) (const IntermediateVec*, void*);

typedef std::vector<std::vector<IntermediatePair>*> MidVectors;

class GlobalContext {
 private:
    int multi_thread_level;
    std::vector<pthread_t*> threads;
    std::vector<ThreadContext*> contexts;
    std::atomic<uint32_t> next_pair_index {0};
    std::atomic<uint32_t> intermediary_elements_number {0};
    std::atomic<uint32_t> progress_counter {0};
    int pairs_number;
    stage_t stage;
 public:
    pthread_mutex_t output_vec_mutex = PTHREAD_MUTEX_INITIALIZER;
    MidVectors shuffled_vectors;
    OutputVec output_vec;
    InputVec input_vec;
    Barrier* threads_barrier;
    const MapReduceClient* client;

    GlobalContext (const MapReduceClient &client, const InputVec &inputVec,
                 OutputVec &outputVec, int multiThreadLevel);
  void start_job ();
  void* job_manager (void* arg);
  void map_manager (ThreadContext* tc);
  void sort_mid_vector (ThreadContext* tc);
  void shuffle_manager (ThreadContext* tc);
  void reduce_manager ();
  K2* find_max_k2_from_threads_vectors ();
  int get_pairs_number () const { return this->pairs_number; }
  bool is_intermediary_keys_equal (K2* key1, K2* key2);
  static int compare_k2(const IntermediatePair& p1, const IntermediatePair& p2);
  void reset_counters ();
  stage_t get_stage() { return stage; }
  void set_stage(stage_t new_stage) { this->stage = new_stage; }
  float get_progress_percentage() { return (float) this->progress_counter / (float) this->pairs_number; }
};

#endif //_GLOBALCONTEXT_H_
