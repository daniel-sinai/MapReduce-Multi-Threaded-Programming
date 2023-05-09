#include "ThreadContext.h"

ThreadContext::ThreadContext (int threadID, std::atomic<uint32_t>* next_pair_index,
                              std::atomic<uint32_t>* progress_counter, GlobalContext* global_context)
{
  this->threadID = threadID;
  this->input_counter = input_counter;
  this->mid_counter = mid_counter;
  this->output_counter = output_counter;
}