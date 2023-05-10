#include "ThreadContext.h"

ThreadContext::ThreadContext (int threadID, GlobalContext* global_context)
{
  this->thread_id = threadID;
  this->global_context = global_context;
}