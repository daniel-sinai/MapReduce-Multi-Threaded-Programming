#include "Barrier.h"

Barrier::Barrier(int numThreads)
		: mutex(PTHREAD_MUTEX_INITIALIZER)
		, cv(PTHREAD_COND_INITIALIZER)
		, count(INITIAL_COUNT)
		, numThreads(numThreads)
{ }


Barrier::~Barrier()
{
	if (pthread_mutex_destroy(&mutex) != SUCCESS_CODE) {
		fprintf(stderr, PTHREAD_MUTEX_DESTROY_FAILED);
		exit(EXIT_ERROR_CODE);
	}
	if (pthread_cond_destroy(&cv) != SUCCESS_CODE){
		fprintf(stderr, PTHREAD_COND_DESTROY_FAILED);
		exit(EXIT_ERROR_CODE);
	}
}


void Barrier::barrier()
{
	if (pthread_mutex_lock(&mutex) != SUCCESS_CODE){
		fprintf(stderr, PTHREAD_MUTEX_LOCK_ERROR);
		exit(EXIT_ERROR_CODE);
	}
	if (++count < numThreads) {
		if (pthread_cond_wait(&cv, &mutex) != SUCCESS_CODE){
			fprintf(stderr, PTHREAD_COND_WAIT_ERROR);
			exit(EXIT_ERROR_CODE);
		}
	} else {
		count = 0;
		if (pthread_cond_broadcast(&cv) != SUCCESS_CODE) {
			fprintf(stderr, PTHREAD_COND_DESTROY_BROADCAST);
			exit(EXIT_ERROR_CODE);
		}
	}
	if (pthread_mutex_unlock(&mutex) != SUCCESS_CODE) {
		fprintf(stderr, PTHREAD_MUTEX_UNLOCK_FAILED);
		exit(EXIT_ERROR_CODE);
	}
}
