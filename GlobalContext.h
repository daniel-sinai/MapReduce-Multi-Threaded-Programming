#include <stdlib.h>
#include <pthread.h>
#include <cstdio>
#include <pthread.h>
#include <vector>
#include <algorithm>
#include "MapReduceClient.h"
#include "ThreadContext.h"
#include "utils.h"
#include "Barrier.h"

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
  int input_vec_len;
  Barrier* threads_barrier;
 public:
  GlobalContext (const MapReduceClient &client, const InputVec &inputVec,
                 OutputVec &outputVec, int multiThreadLevel);

};

#endif //_GLOBALCONTEXT_H_
