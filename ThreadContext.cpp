#include "ThreadContext.h"

ThreadContext::ThreadContext (int threadID, std::atomic<uint32_t>* next_pair_index,
                              std::atomic<uint32_t>* progress_counter, GlobalContext* global_context)
{
  this->thread_id = threadID;
  this->next_pair_index = next_pair_index;
  this->progress_counter = progress_counter;
  this->global_context = global_context;
}