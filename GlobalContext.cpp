#include "GlobalContext.h"

GlobalContext::GlobalContext (const MapReduceClient &client,
                              const InputVec &inputVec, OutputVec &outputVec,
                              int multiThreadLevel)
{
  for (int i = 0; i < multiThreadLevel; ++i) {
      ThreadContext *new_context = new ThreadContext(i, &this->atomic_counter);
      contexts.push_back (new_context);
    }

  for (int i = 0; i < multiThreadLevel; ++i) {
      pthread_t *new_thread = new pthread_t();
      pthread_create(new_thread, NULL, nullptr, contexts[i]);
    }

  for (int i = 0; i < multiThreadLevel; ++i) {
      pthread_join(*(threads[i]), NULL);
    }
}