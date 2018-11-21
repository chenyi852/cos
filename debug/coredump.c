#include "elf.h"
#include "elf_core.h"
#include "coredump.h"
#include "types.h"

#include <stdio.h>
#include <stdlib.h>

#define dlog printf



/* 
 * core dump function 
 *
 */
int elf_core_dump(struct core_dump_params *cdprms)
{
	struct elfhdr *elf = NULL;
	int segs;
	struct elf_note_info info;	
	/* 1 loc memory for elf header */
	elf = (struct elfhdr*)malloc(sizeof(*elf));
	if (!elf) {
		dlog("malloc fail!\n");
		return -1;		
	}

	/* 2 count the segment number, including note adn load segments */
	segs = cdprms-> segments;

	/* 3 fill note segment */
	get_thread_info();
	/* 4 fill load segment */

	free(elf);

	return 0;
}
