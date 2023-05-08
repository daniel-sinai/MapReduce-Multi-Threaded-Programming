#include "GlobalContext.h"

GlobalContext::GlobalContext (const MapReduceClient &client,
                              const InputVec &inputVec, OutputVec &outputVec,
                              int multiThreadLevel)
{
    this->input_vec = inputVec;
    this->output_vec = outputVec;
    this->client = &client;
    this->input_vec_len = inputVec.size ();
    this->threads_barrier = new(std::nothrow) Barrier(multiThreadLevel);
    if (this->threads_barrier == nullptr)
    {
        fprintf(stderr, MEMORY_ALLOC_FAILED);
        exit(EXIT_ERROR_CODE);
    }
  for (int i = 0; i < multiThreadLevel; ++i) {
      ThreadContext *new_context = new ThreadContext(i, &this->atomic_counter);
      contexts.push_back (new_context);
    }

  for (int i = 0; i < multiThreadLevel; ++i) {
      pthread_t *new_thread = new(std::nothrow) pthread_t();
      if (new_thread == nullptr)
      {
          // TODO free system
          fprintf(stderr, MEMORY_ALLOC_FAILED);
          exit(EXIT_ERROR_CODE);
      }
      if (pthread_create(new_thread, NULL, nullptr, contexts[i]) != SUCCESS_CODE)
      {
          // TODO free system
          fprintf(stderr, PTHREAD_CREATE_FAILED);
          exit(EXIT_ERROR_CODE);
      }
      threads.push_back(new_thread);
    }

//  for (int i = 0; i < multiThreadLevel; ++i) {
//      pthread_join(*(threads[i]), NULL);
//    }
}

void GlobalContext::job_manager () {
    // map phase till end list
    // wait for all map phase
    // wait for shuffle
    // reduce phase till end list
    // terminate
}

void GlobalContext::map_manager() {

}