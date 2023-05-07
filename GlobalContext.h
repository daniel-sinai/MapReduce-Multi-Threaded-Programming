#include <stdlib.h>
#include <pthread.h>
#include <cstdio>
#include <atomic>
#include <pthread.h>
#include <vector>
#include "MapReduceClient.h"
#include "ThreadContext.h"

#ifndef _GLOBALCONTEXT_H_
#define _GLOBALCONTEXT_H_

class GlobalContext {
 private:
  std::vector<pthread_t*> threads;
  std::vector<ThreadContext*> contexts;
  std::atomic<uint32_t> atomic_counter {0};
 public:
  GlobalContext (const MapReduceClient &client, const InputVec &inputVec,
                 OutputVec &outputVec, int multiThreadLevel);

};

#endif //_GLOBALCONTEXT_H_
