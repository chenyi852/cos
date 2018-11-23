#include "elf.h"
#include "elf_core.h"
#include "coredump.h"
#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#define dlog printf

static void fill_elf_header(struct elfhdr *elf, int segs,
			    u16 machine, u32 flags)
{
	memset(elf, 0, sizeof(*elf));

	memcpy(elf->e_ident, ELFMAG, SELFMAG);
	elf->e_ident[EI_CLASS] = ELF_CLASS;
	elf->e_ident[EI_DATA] = ELF_DATA;
	elf->e_ident[EI_VERSION] = EV_CURRENT;
	elf->e_ident[EI_OSABI] = ELF_OSABI;

	elf->e_type = ET_CORE;
	elf->e_machine = machine;
	elf->e_version = EV_CURRENT;
	elf->e_phoff = sizeof(struct elfhdr);
	elf->e_flags = flags;
	elf->e_ehsize = sizeof(struct elfhdr);
	elf->e_phentsize = sizeof(struct elf_phdr);
	elf->e_phnum = segs;

	return;
}


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
