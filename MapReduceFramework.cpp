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

void emit2 (K2* key, V2* value, void* context) {
    ThreadContext* tc = (ThreadContext*) context;
    tc->append_to_vector(std::make_pair(key, value));
    tc->increment_progress_counter ();
}

void getJobState(JobHandle job, JobState* state)
{
    GlobalContext* global_context = (GlobalContext* ) job;
    state->stage = global_context->get_stage ();
    state->percentage = global_context->get_progress_percentage ();
}

void closeJobHandle(JobHandle job)
{
    return;
}

void waitForJob(JobHandle job)
{
    return;
}

void emit3 (K3* key, V3* value, void* context)
{
    return;
}