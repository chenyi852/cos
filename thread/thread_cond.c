/*
 * chenyi852@gmail.com
 * 2018-09-12
 */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "thread.h"
/*  --------------------------------
	T_a		T-b
	wait		++1
	|		|
	|		|
	print i  <----	if i is a
			mulitple of 3
    ---------------------------------
*/

/* initilize the mutex */
pthread_mutex_t	mutex	= PTHREAD_MUTEX_INITIALIZER;
/* init cond */
pthread_cond_t	cond	= PTHREAD_COND_INITIALIZER;

int i = 1;

void *threada(void *);
void *threadb(void *);

void *threada(void *junk)
{
	while (i < 9){
		pthread_mutex_lock(&mutex);
		if (i %3 != 0) {
			/* wait for the sinal from thread b */
			pthread_cond_wait(&cond, &mutex);
		}
		PRN_DECS32(i);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	
	PRN_EXIT("exit");
}

void *threadb(void *junk)
{
	for (i = 1; i <= 9; i++) {

		pthread_mutex_lock(&mutex);
		if (i %3 ==0){
			/* send signal to thread a */
			pthread_cond_signal(&cond);
		} else {
			PRN_DECS32(i);
		}
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}

	PRN_EXIT("exit");
}

int test_pthread_cond(void)
{
	pthread_t t_a;
	pthread_t t_b;
	long *ret_a;
	long *ret_b;

	pthread_create(&t_a, NULL, threada, (void *)NULL);
	pthread_create(&t_b, NULL, threadb, (void *)NULL);

	pthread_join(t_b, (void *)&ret_b);
	printf("[%s]exit with errcode %lx\n", __func__, (long)ret_b);
	pthread_join(t_a, (void *)&ret_a);
	printf("[%s]exit with errcode %lx\n", __func__, (long)ret_a);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	return 0;
}
