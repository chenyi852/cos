#include <stdio.h>

#include "bit.h"
#include "test.h"

void test_lib(void)
{
	u32 num=0x82;

	printf("ffs(%x) = %d\n", num, ffs(num));
}
