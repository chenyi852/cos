#include <stdio.h>

#include "hm_va.h"

extern int clz(unsigned int x);
int main(int argc, char * argv[])
{
	unsigned int number = 0x4;

	printf("clz(%d) = %d\n", number, clz(number));
	printf("%.*s--- ~0ULL = 0x%llx\n", 5, "----------------", (~0ULL));

	va_entry();
	return 0;
}
