#include <stdio.h>


int clz(unsigned int x)
{

	return __builtin_clz(x);
}


