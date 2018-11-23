#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

#include "elf.h"
#include "elf_core.h"
#include "coredump.h"
#include "types.h"


#define codp_log printf
#define CORE_FILE	"core.elf"

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

static void fill_core_info(struct elfhdr *elf, int segs)
{

	/* 1 write elf header */
	fill_elf_header(elf, segs, ELF_ARCH, ELF_CORE_EFLAGS); 

}

static void write_core(FILE *fp, const void *addr, size_t sz)
{
	return fwrite(addr, sz, 1, fp);
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
		codp_log("malloc fail!\n");
		return -1;		
	}

	/* 2 count the segment number, including note adn load segments */
	cdprms->segments = 3;
	segs = cdprms-> segments;
	

	/* 3 fill note segment */
	fill_core_info(elf, segs);
	write_core(cdprms->fp, elf, sizeof(*elf));

	get_thread_info();
	/* 4 fill load segment */

	free(elf);

	return 0;
}

int create_elf_core(void)
{
	FILE *fd;
	struct core_dump_params cdprms;
	int ret = 0;

	do {
		fd = fopen(CORE_FILE, "w+");
		if (!fd) {
			codp_log("create file %s fail\n", CORE_FILE);
			return -1;
		}

		cdprms.fp = fd;

		ret = elf_core_dump(&cdprms);
		if (ret != 0) {
			codp_log("write core file!@%d\n", ret);
			break;
		}

	}while(0);

	fclose(fd);
	
	return ret;
}
