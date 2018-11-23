#ifndef _COS_CORED_DUMP_H
#define _COS_CORED_DUMP_H

#include "elf.h"

struct core_dump_params {
	FILE *fp;
	int segments; /* number of load segments */
};

int create_elf_core(void);
#endif
