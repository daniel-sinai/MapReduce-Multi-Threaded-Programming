#include "MapReduceFramework.h"
#include "ThreadContext.h"
#include "MapReduce.h"
#include "GlobalContext.h"
#include <utility>


JobHandle
startMapReduceJob (const MapReduceClient &client, const InputVec &inputVec,
                   OutputVec &outputVec, int multiThreadLevel)
{
  GlobalContext* global_context = new GlobalContext(client, inputVec, outputVec, multiThreadLevel);
  MapReduce::start_job (multiThreadLevel, global_context);
  return global_context;
}

void emit2 (K2* key, V2* value, void* context) {
    ThreadContext* tc = (ThreadContext*) context;
    GlobalContext* gc = tc->global_context;
    tc->append_to_map_vector(std::make_pair(key, value));
    gc->increment_intermediary_elements_number();
}

void getJobState(JobHandle job, JobState* state)
{
    GlobalContext* global_context = (GlobalContext* ) job;
    state->stage = global_context->get_stage ();
    switch (state->stage) {
        case UNDEFINED_STAGE:
            break;

        case SHUFFLE_STAGE:
        case REDUCE_STAGE:
            state->percentage = global_context->get_shuffle_progress_percentage ();
            break;

        case MAP_STAGE:
            state->percentage = global_context->get_map_progress_percentage();
            break;

        default:
            break;
    }
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
    ThreadContext* tc = (ThreadContext*) context;
    GlobalContext* gc = tc->global_context;
//    pthread_mutex_lock(&gc->output_vec_mutex);
    gc->output_vec.push_back(std::make_pair(key, value));
//    pthread_mutex_unlock(&gc->output_vec_mutex);
    gc->increment_progress_counter();
    return;
}