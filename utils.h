#ifndef MAPREDUCE_MULTI_THREADED_PROGRAMMING_UTILS_H
#define MAPREDUCE_MULTI_THREADED_PROGRAMMING_UTILS_H

#define MEMORY_ALLOC_FAILED "system error: memory allocation failed\n"
#define PTHREAD_CREATE_FAILED "system error: pthread_create failed\n"
#define PTHREAD_MUTEX_DESTROY_FAILED "[[Barrier]] error on pthread_mutex_destroy\n"
#define PTHREAD_COND_DESTROY_FAILED "[[Barrier]] error on pthread_cond_destroy\n"
#define PTHREAD_MUTEX_UNLOCK_FAILED "[[Barrier]] error on pthread_mutex_unlock\n"
#define PTHREAD_MUTEX_LOCK_ERROR "[[Barrier]] error on pthread_mutex_lock\n"
#define PTHREAD_COND_DESTROY_BROADCAST "[[Barrier]] error on pthread_cond_broadcast\n"
#define PTHREAD_COND_WAIT_ERROR "[[Barrier]] error on pthread_cond_wait\n"
#define PTHREAD_JOIN_FAILED "system error: error on pthread_join\n"
#define SUCCESS_CODE 0
#define EXIT_ERROR_CODE 1
#define INITIAL_COUNT 0
#endif //MAPREDUCE_MULTI_THREADED_PROGRAMMING_UTILS_H
