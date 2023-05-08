#include "ThreadContext.h"
#include <atomic>

ThreadContext::ThreadContext (int threadID, std::atomic<uint32_t>* input_counter,
                              std::atomic<uint32_t>* mid_counter, std::atomic<uint32_t>* output_counter)
{
  this->threadID = threadID;
  this->input_counter = input_counter;
  this->mid_counter = mid_counter;
  this->output_counter = output_counter;
}