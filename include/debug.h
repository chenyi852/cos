#ifndef _CDEBUG_H
#define _CDEBUG_H

#include <stdio.h>

#define PRN_DEC32(x) \
	printf("[%s]%-32s = %u\n", __func__, #x, x)
#define PRN_DECS32(x) \
	printf("[%s]%-32s = %d\n", __func__, #x, x)

#define PRN_HEX64(x) \
	printf("[%s]%-32s = 0x%016lx\n", __func__, #x, x)
	

#define MSTAR \
	"**************"

#define PRN_ENTER(x) \
	printf("[%s]%s %s %s\n", __func__, MSTAR, x, MSTAR);
#endif

#define PRN_EXIT(x) PRN_ENTER(x)
