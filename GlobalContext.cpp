#include "GlobalContext.h"

GlobalContext::GlobalContext (const MapReduceClient &client,
                              const InputVec &input_vec, OutputVec &output_vec,
                              int multiThreadLevel) :
    client (client), input_vec (input_vec), output_vec (output_vec)
{
  this->pairs_number = (int) input_vec.size ();
  this->set_stage_and_reset_general_atomic(UNDEFINED_STAGE);
  this->threads_barrier = new Barrier (multiThreadLevel);
  this->intermediary_elements = 0;
}

GlobalContext::~GlobalContext ()
{
  delete threads_barrier;
}

void GlobalContext::increment_first_counter_general_atomic (uint64_t inc)
{
  general_atomic.fetch_add (inc << 2);
}

void GlobalContext::increment_second_counter_general_atomic (uint64_t inc)
{
  general_atomic.fetch_add ((inc << 33));
}

stage_t GlobalContext::get_stage ()
{
  return (stage_t) (general_atomic.load () & 3);
}

uint64_t GlobalContext::get_first_counter_value ()
{
  auto mask = (uint64_t) (general_atomic.load () & 8388604);
  return mask >> 2;
}

uint64_t GlobalContext::get_second_counter_value ()
{
  uint64_t mask = general_atomic.load () & (~8589934591);
  return mask >> 33;
}

void GlobalContext::set_stage_and_reset_general_atomic (
    stage_t stage)
{
  general_atomic.exchange (stage);
}