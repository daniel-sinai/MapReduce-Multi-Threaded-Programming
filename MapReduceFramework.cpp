#include "MapReduceFramework.h"
#include "ThreadContext.h"
#include "GlobalContext.h"
#include <pthread.h>
#include <utility>


JobHandle
startMapReduceJob (const MapReduceClient &client, const InputVec &inputVec,
                   OutputVec &outputVec, int multiThreadLevel)
{
  GlobalContext* global_context = new GlobalContext(client, inputVec, outputVec, multiThreadLevel);
  return global_context;
}
