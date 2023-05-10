#ifndef MAPREDUCE_MULTI_THREADED_PROGRAMMING_MAPREDUCE_H
#define MAPREDUCE_MULTI_THREADED_PROGRAMMING_MAPREDUCE_H

#include "GlobalContext.h"
#include "ThreadContext.h"

class MapReduce {
private:
    static std::vector<pthread_t*> threads;
    static std::vector<ThreadContext*> contexts;
public:
    MapReduce () = delete;
    static void start_job (int multi_thread_level, GlobalContext* global_context);
    static void* job_manager (void* arg);
    static void map_manager (ThreadContext* tc, GlobalContext* gc);
    static void sort_mid_vector (ThreadContext* tc);
    static void shuffle_manager (ThreadContext* tc, GlobalContext* gc);
    static void reduce_manager (ThreadContext* tc, GlobalContext* gc);
    static K2* find_max_k2_from_threads_vectors ();
    static bool is_intermediary_keys_equal (K2* key1, K2* key2);
    static bool compare_k2(const IntermediatePair& p1, const IntermediatePair& p2);
};
#endif //MAPREDUCE_MULTI_THREADED_PROGRAMMING_MAPREDUCE_H
