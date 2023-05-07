#include "ThreadContext.h"
#include <atomic>

ThreadContext::ThreadContext (int threadID, std::atomic<uint32_t>* atomic_counter)
{
  this->threadID = threadID;
  this->atomic_counter = atomic_counter;
}