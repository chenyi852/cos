/*
 * chenyi852@gmail.com
 * 2018-09-12
 */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "thread.h"

int thread_entry(void)
{
	test_pthread_cond();
}
