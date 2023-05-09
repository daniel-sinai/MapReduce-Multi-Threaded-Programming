#ifndef EX3_THREADCONTEXT_H
#define EX3_THREADCONTEXT_H

#include <atomic>
#include <vector>
#include "MapReduceClient.h"
#include "GlobalContext.h"

class ThreadContext {
 private:
    int thread_id;
    std::atomic<uint32_t>* next_pair_index;
    std::atomic<uint32_t>* progress_counter;
    uint32_t curr_vector_id;
    std::vector<IntermediatePair> vector;
    GlobalContext* global_context;

 public:
    ThreadContext (int threadID, std::atomic<uint32_t>* next_pair_index, std::atomic<uint32_t>* progress_counter, GlobalContext* global_context);
    std::atomic<uint32_t>* get_input_counter () { return this->next_pair_index; }
    uint32_t increment_next_pair_index () { return (*(this->next_pair_index))++; };
    uint32_t increment_progress_counter () { return (*(this->progress_counter))++; };
    void set_curr_vector_id (uint32_t id) { this->curr_vector_id = id; }
    uint32_t get_curr_vector_id () { return this->curr_vector_id; }
    void append_to_vector (IntermediatePair to_append) { this->vector.push_back (to_append); }
    std::vector<IntermediatePair>* get_vector () { return &(this->vector); }
    int get_thread_id () const { return this->thread_id; }
};

#endif //EX3_THREADCONTEXT_H
