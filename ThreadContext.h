#ifndef EX3_THREADCONTEXT_H
#define EX3_THREADCONTEXT_H

#include <atomic>
#include <vector>
#include "MapReduceClient.h"
#include "GlobalContext.h"

class ThreadContext {
 private:
  int thread_id;

 public:
  int curr_reduce_vector_size;
  IntermediateVec map_vector;
  ThreadContext (int threadID, GlobalContext *global_context);
  GlobalContext *global_context;

  // Getters
  int get_thread_id () const
  { return this->thread_id; }
};

#endif //EX3_THREADCONTEXT_H
