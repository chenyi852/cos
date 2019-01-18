#ifndef _COS_CORED_DUMP_H
#define _COS_CORED_DUMP_H

#include "elf.h"

/* The `const' in roundup() prevents gcc-3.3 from calling __divdi3 */
#define roundup(x, y) (					\
{							\
	const typeof(y) __y = y;			\
	(((x) + (__y - 1)) / __y) * __y;		\
}

struct core_dump_params {
	FILE *fp;
	int segments; /* number of load segments */
	pt_regs *regs;
};

int create_elf_core(void);
#endif
