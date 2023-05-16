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

GlobalContext::~GlobalContext ()
{
  delete threads_barrier;
}

void GlobalContext::increment_first_counter_general_atomic (uint64_t inc)
{
  uint64_t new_value = general_atomic.load ();
  new_value += (inc << 2);
  general_atomic.exchange (new_value);
}

void GlobalContext::increment_second_counter_general_atomic (uint64_t inc)
{
  uint64_t new_value = general_atomic.load ();
  new_value += (inc << 33);
  general_atomic.exchange (new_value);
}

stage_t GlobalContext::get_stage ()
{
  return (stage_t) (general_atomic.load () & 3);
}

uint64_t GlobalContext::get_first_counter_value ()
{
  uint64_t mask = (uint64_t) (general_atomic.load () & 8388604);
  return mask >> 2;
}

uint64_t GlobalContext::get_second_counter_value ()
{
  uint64_t mask = general_atomic.load () & 18446744065119617024;
  return mask >> 33;
}

void GlobalContext::set_stage_and_reset_general_atomic (
    stage_t stage, bool reset_first, bool reset_second)
{
  uint64_t new_value = general_atomic.load ();
  new_value = new_value & (~3);
  new_value = stage | stage;
  if (reset_first)
    {
      new_value = new_value & (~8388604);
    }
  if (reset_second)
    {
      new_value = new_value & (~18446744065119617024);
    }
  general_atomic.exchange (new_value);
}

