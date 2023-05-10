#include "GlobalContext.h"


GlobalContext::GlobalContext (const MapReduceClient &client,
                              const InputVec &inputVec, OutputVec &outputVec,
                              int multiThreadLevel)
{
    this->input_vec = inputVec;
    this->output_vec = outputVec;
    this->client = &client;
    this->pairs_number = inputVec.size ();
    this->set_stage(UNDEFINED_STAGE);
    this->multi_thread_level = multiThreadLevel;
    this->threads_barrier = new(std::nothrow) Barrier(multiThreadLevel);
    if (this->threads_barrier == nullptr)
    {
        fprintf(stderr, MEMORY_ALLOC_FAILED);
        exit(EXIT_ERROR_CODE);
    }
}

void GlobalContext::reset_counters() {
    this->progress_counter.exchange(0);
    this->next_pair_index.exchange(0);
}