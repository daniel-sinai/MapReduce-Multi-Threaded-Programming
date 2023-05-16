#ifndef EX3_THREADCONTEXT_H
#define EX3_THREADCONTEXT_H

#include <atomic>
#include <vector>
#include "MapReduceClient.h"
#include "GlobalContext.h"

class ThreadContext {
 private:
  int thread_id;
  int input_vector_id;
  int shuffle_vector_id;
 public:
  int curr_reduce_vector_size;
  IntermediateVec map_vector;
  ThreadContext (int threadID, GlobalContext *global_context);
  GlobalContext *global_context;

  // Getters
  int get_input_vector_id ()
  { return this->input_vector_id; }
  int get_thread_id () const
  { return this->thread_id; }
  int get_shuffle_vector_id ()
  { return shuffle_vector_id; }

  // Setters
  void set_input_vector_id (int id)
  { this->input_vector_id = id; }
  void set_shuffle_vector_id (int id)
  { this->shuffle_vector_id = id; }

  // Vectors
  void append_to_map_vector (IntermediatePair to_append)
  { this->map_vector.push_back (to_append); }
};

#endif //EX3_THREADCONTEXT_H
