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

//typedef void (*map_function) (const K1*, const V1*, void*);
//typedef void (*reduce_function) (const IntermediateVec*, void*);
// daniel is gay 1

class GlobalContext {
 private:
  std::vector<pthread_t*> threads;
  std::vector<ThreadContext*> contexts;
  std::atomic<uint32_t> atomic_counter {0};
  OutputVec output_vec;
  InputVec input_vec;
  const MapReduceClient* client;
 public:
  GlobalContext (const MapReduceClient &client, const InputVec &inputVec,
                 OutputVec &outputVec, int multiThreadLevel);

};

#endif //_GLOBALCONTEXT_H_
