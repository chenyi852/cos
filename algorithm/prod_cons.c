#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#include "algorithm.h"
#include "types.h"
#include "prod_cons.h"

/* global semphore for syncronization */
sem_t sem_empty, sem_full;
/* global mutex between producer and consumer */
pthread_mutex_t mutex_src;
/* global variable to stand for product number in the channel */
int buffer_count = 0;

void *producer(void *arg);
void *consumer(void *arg);

int test_prod_cons(void)
{
	pthread_t thrd_prod, thrd_cons;

	pthread_mutex_init(&mutex_src, NULL);

	sem_init(&sem_empty, 0, 5);
	sem_init(&sem_full, 0, 0);

	/* spawn producer thread */
	if (pthread_create(&thrd_prod, NULL, producer, NULL) != 0) {
		printf("create producer thread fail!\n");
		return -1;
	
	}

	if (pthread_create(&thrd_cons, NULL, consumer, NULL)!= 0) {
		printf("create consumer thread fail!\n");
		return -1;

	}

	if (pthread_join(thrd_prod, NULL) != 0) {
		printf("wait for thread producer fail!\n");
	}

	if (pthread_join(thrd_cons, NULL) != 0) {
		printf("wait for thread consumer fail!\n");
	}

	sem_destroy(&sem_full);
	sem_destroy(&sem_empty);
	pthread_mutex_destroy(&mutex_src);

	return 0;
}


void *producer(void *arg) {
	int count = 0;

	do {
		/* sem_empty - 1, later, empty slot will be taken */
		sem_wait(&sem_empty);
		/* get the mutex lock and then change the global resources */
		pthread_mutex_lock(&mutex_src);
		buffer_count++;
		pthread_mutex_unlock(&mutex_src);
		/* sem_full + 1, an new slot has beend load some resources  */
		sem_post(&sem_full);
		if ( count % 10 == 0)
			printf("producer has procudes %d buffer!\n", count);
	} while (count++ < 1000); 

}

void *consumer(void *arg) {
	int count = 0;

	do {
		/* sem_full - 1, later, full slot will be consumed */
		sem_wait(&sem_full);
		/* get the mutex lock, and then comsum the global resources */
		pthread_mutex_lock(&mutex_src);
		buffer_count--;
		pthread_mutex_unlock(&mutex_src);
		/* an slot was consumed and freed, so sem_empty++ */
		sem_post(&sem_empty);
		if ( count % 10 == 0)
			printf("consumer has procudes %d buffer!\n", count);

	}while (count++ < 1000);
}

/* --------------------------------------------------------------------------
 *  mutex + condition
 *  -------------------------------------------------------------------------
 */

