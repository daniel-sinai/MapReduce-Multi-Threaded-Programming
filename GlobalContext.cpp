#include "GlobalContext.h"

GlobalContext::GlobalContext (const MapReduceClient &client,
                              const InputVec &input_vec, OutputVec &output_vec,
                              int multiThreadLevel) :
    client (client), input_vec (input_vec), output_vec (output_vec)
{
  this->pairs_number = input_vec.size ();
  this->set_stage (UNDEFINED_STAGE);
  this->multi_thread_level = multiThreadLevel;
  this->threads_barrier = new Barrier (multiThreadLevel);
}

void GlobalContext::reset_counters ()
{
  this->progress_counter.exchange (0);
  this->next_pair_index.exchange (0);
}

GlobalContext::~GlobalContext ()
{
  delete threads_barrier;
}