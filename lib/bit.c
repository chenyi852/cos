#include <stdio.h>
#include "bit.h"

int clz(unsigned int x)
{

	return __builtin_clz(x);
}

/* 
 * return one plus the index of least significant 1-bit of x, or if x is zero,
 * return 0
 */
int ffs(unsigned int x)
{
	return __builtin_ffs(x);
}
