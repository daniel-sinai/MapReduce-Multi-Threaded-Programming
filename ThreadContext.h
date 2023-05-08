#include <atomic>
#include <vector>
#include "MapReduceClient.h"

#ifndef EX3_THREADCONTEXT_H
#define EX3_THREADCONTEXT_H

class ThreadContext {
 private:
  int threadID;
  std::atomic<uint32_t>* atomic_counter;

 public:
  ThreadContext (int threadID, std::atomic<uint32_t>* atomic_counter);
};

#endif //EX3_THREADCONTEXT_H
