#include "Barrier.h"
#include <cstdlib>
#include <cstdio>

Barrier::Barrier(int numThreads)
		: mutex(PTHREAD_MUTEX_INITIALIZER)
		, cv(PTHREAD_COND_INITIALIZER)
		, count(INITIAL_COUNT)
		, numThreads(numThreads)
{ }


Barrier::~Barrier()
{
	if (pthread_mutex_destroy(&mutex) != SUCCESS_CODE) {
		fprintf(stderr, "[[Barrier]] error on pthread_mutex_destroy");
		exit(1);
	}
	if (pthread_cond_destroy(&cv) != 0){
		fprintf(stderr, "[[Barrier]] error on pthread_cond_destroy");
		exit(1);
	}
}


void Barrier::barrier()
{
	if (pthread_mutex_lock(&mutex) != SUCCESS_CODE){
		fprintf(stderr, "[[Barrier]] error on pthread_mutex_lock");
		exit(1);
	}
	if (++count < numThreads) {
		if (pthread_cond_wait(&cv, &mutex) != SUCCESS_CODE){
			fprintf(stderr, "[[Barrier]] error on pthread_cond_wait");
			exit(1);
		}
	} else {
		count = 0;
		if (pthread_cond_broadcast(&cv) != SUCCESS_CODE) {
			fprintf(stderr, "[[Barrier]] error on pthread_cond_broadcast");
			exit(1);
		}
	}
	if (pthread_mutex_unlock(&mutex) != SUCCESS_CODE) {
		fprintf(stderr, "[[Barrier]] error on pthread_mutex_unlock");
		exit(1);
	}
}
