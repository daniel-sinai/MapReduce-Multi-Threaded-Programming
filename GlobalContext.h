#ifndef _GLOBALCONTEXT_H_
#define _GLOBALCONTEXT_H_

#include <pthread.h>
#include <cstdio>
#include <pthread.h>
#include <vector>
#include <algorithm>
#include <atomic>
#include <iostream>
#include "MapReduceClient.h"
#include "MapReduceFramework.h"
#include "utils.h"
#include "Barrier.h"

//typedef void (*map_function) (const K1*, const V1*, void*);
//typedef void (*reduce_function) (const IntermediateVec*, void*);

typedef std::vector<std::vector<IntermediatePair>*> MidVectors;

class ThreadContext;

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

    // Getters
    float get_map_progress_percentage () { return (float) this->progress_counter / (float) this->pairs_number * 100.0; }
    float get_shuffle_progress_percentage () { return (float) this->progress_counter / (float) intermediary_elements_number * 100.0; }
    int get_pairs_number () const { return this->pairs_number; }
    stage_t get_stage() { return stage; }

    // Setters
    void set_stage(stage_t new_stage) { this->stage = new_stage; }

    // Atmoic variables
    uint32_t increment_next_pair_index () { return (this->next_pair_index)++; }
    uint32_t increment_progress_counter (int inc=1) { return this->progress_counter.fetch_add(inc); }
    uint32_t increment_intermediary_elements_number () { return (this->intermediary_elements_number)++; }
    void reset_counters ();
};

#endif //_GLOBALCONTEXT_H_
