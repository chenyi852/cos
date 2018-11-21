#ifndef _COS_CORED_DUMP_H
#define _COS_CORED_DUMP_H

#include "elf.h"

struct core_dump_params {
	int segments; /* number of load segments */
};


int elf_core_dump(struct core_dump_params *cdprm);

#endif
