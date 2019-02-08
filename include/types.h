#ifndef _CTYPE_H
#define _CTYPE_H

#include <stdint.h>
#include "arch_types.h"
#include <stdio.h>

typedef char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef int s32;
typedef unsigned int u32;

typedef long  s64;
typedef unsigned long u64;

typedef size_t foffset;

struct list_head {
	struct list_head *next, *prev;
};
#endif
