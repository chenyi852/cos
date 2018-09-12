#ifndef _CDEBUG_H
#define _CDEBUG_H

#include <stdio.h>

#define PRN_DEC32(x) \
	printf("%-32s = %u\n", #x, x)

#define PRN_HEX64(x) \
	printf("%-32s = 0x%016lx\n", #x, x)
	

#define MSTAR \
	"**************"

#define PRN_ENTER(x) \
	printf("%s %s %s\n", MSTAR, x, MSTAR);
#endif
