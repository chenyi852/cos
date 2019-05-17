#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <sys/user.h> /* structure definition */
#include <sys/reg.h> /* macro for regs */

#include "arch_thread.h"
#include "elf.h"
#include "elf_core.h"
#include "coredump.h"
#include "types.h"


#define codp_log printf
#define CORE_FILE	"core.elf"

static size_t write_core(FILE *fp, const void *addr, size_t sz)
{
	return fwrite(addr, sz, 1, fp);
}

static int fill_elf_header(int segs, u16 machine, u32 flags, FILE *fp)
{
	struct elfhdr *elf;

	elf = (struct elfhdr *)malloc(sizeof(*elf));
	if (!elf) {
		codp_log("[%s]malloc fail!\n", __func__);
		return -1;
	}
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

	/* write elf header to core files */
	write_core(fp, elf, sizeof(*elf));

	free(elf);

	return 0;
}

static int fill_elf_note_phdr(int sz, loff_t offset, FILE *fp)
{
	struct elf_phdr *phdr;

	phdr = malloc(sizeof(*phdr));
	if (!phdr) {
		codp_log("[%s]malloc fail\n", __func__);
		return -1;
	}


	phdr->p_type = PT_NOTE;
	phdr->p_offset = offset;
	phdr->p_vaddr = 0;
	phdr->p_paddr = 0;
	phdr->p_filesz = sz;
	phdr->p_memsz = 0;
	phdr->p_flags = 0;
	phdr->p_align = 0;

	write_core(fp, phdr, sizeof(*phdr));

	free(phdr);

	return 0;
}

#if 0
static int writenote(struct memelfnote *men, FILE *fp,
			loff_t *foffset)
{

	return 0;
}
#endif

#if 0
static int write_note_info(struct elf_note_info *info,
			   FILE *fp, loff_t *foffset)
{
	int i;
	/* struct list_head *t; */
	for (i = 0; i < info->numnote; i++)
		if (!writenote(info->notes + i, fp, foffset))
			return 1;

#if 0
	/* write out the thread status notes section */
	list_for_each(t, &info->thread_list) {
		struct elf_thread_status *tmp =
				list_entry(t, struct elf_thread_status, list);

		for (i = 0; i < tmp->num_notes; i++)
			if (!writenote(&tmp->notes[i], file, foffset))
				return 1;
	}
#endif
	return 0;
}
#endif

#if 0
static void fill_note(struct memelfnote *note, const char *name, int type,
		unsigned int sz, void *data)
{
	note->name = name;
	note->type = type;
	note->datasz = sz;
	note->data = data;

	return;
}
#endif

/*
 * @ fill the program header of load segment
 * @ filesz load file size
 * @ offset offset of the load segment in the core file
 * @ fp the handler of core file
 * @ return : size written to the core file
 */
static size_t fill_elf_load_phdr(int filesz, loff_t offset, FILE *fp)
{
	struct elf_phdr phdr;
	size_t wt_sz =0;

	phdr.p_type	= PT_LOAD;
	phdr.p_offset	= offset;
	phdr.p_vaddr	= 0;
	phdr.p_paddr	= 0;
	phdr.p_filesz	= filesz;
	phdr.p_memsz	= 0;
	phdr.p_flags	= 0;
	phdr.p_align	= 0;

	wt_sz = write_core(fp, &phdr, sizeof(phdr));

	return wt_sz;
}

static int elf_note_info_init(struct elf_note_info *info)
{
	memset(info, 0, sizeof(*info));

	/* Allocate space for ELF notes */
	info->notes = malloc(8 * sizeof(struct memelfnote));
	if (!info->notes)
		return -1;
	info->psinfo = malloc(sizeof(*info->psinfo));
	if (!info->psinfo)
		return -1;
	info->prstatus = malloc(sizeof(*info->prstatus));
	if (!info->prstatus)
		return -1;
	info->fpu = malloc(sizeof(*info->fpu));
	if (!info->fpu)
		return -1;
#ifdef ELF_CORE_COPY_XFPREGS
	info->xfpu = malloc(sizeof(*info->xfpu));
	if (!info->xfpu)
		return -1;
#endif
	return 0;
}

static void free_note_info(struct elf_note_info *info)
{

	free(info->prstatus);
	free(info->psinfo);
	free(info->notes);
	free(info->fpu);
#ifdef ELF_CORE_COPY_XFPREGS
	free(info->xfpu);
#endif
}

static void fill_prstatus(struct elf_prstatus *prstatus,
		pt_regs *regs)
{
	memset(prstatus, 0, sizeof(*prstatus));
	ELF_CORE_COPY_REGS(prstatus->pr_reg, regs);
}

/*
 * @ foffset
 * @ fp the handler of core file
 * @ return: size written to the core file
 */
static size_t fill_note_info(loff_t foffset, struct core_dump_params *cdpm)
{
	struct elf_note_info info = { };
	/* FILE *fp	= cdpm->fp; */
	pt_regs *regs	= cdpm->regs;
	size_t wt_sz =0;

	do {
		if (elf_note_info_init(&info) != 0)
		{
			wt_sz = 0;
			break;
		}

		fill_prstatus(info.prstatus, regs);

	} while (0);

	free_note_info(&info);

	return wt_sz;
}

static void fill_core_info(struct core_dump_params *cdpm)
{
	FILE *fp = cdpm->fp;
	int segs = cdpm->segments;
	loff_t offset = 0, foffset;

	/* 1 write elf header */
	fill_elf_header(segs, ELF_ARCH, ELF_CORE_EFLAGS, fp);

	/* 2 caculate the offset of the note segments */
	offset += sizeof(struct elfhdr);
	offset += segs * sizeof(struct elf_phdr);
	foffset = offset;

	/* 3 write note phdr */
	fill_elf_note_phdr(0x1200, offset, fp);

	/*  4 write load phdr(vma) align to page */
	fill_elf_load_phdr(0x1200, offset, fp);

	fill_elf_load_phdr(0x2400, offset, fp);

	fill_note_info(foffset, cdpm);
}


/*
 * core dump function
 *
 */
int elf_core_dump(struct core_dump_params *cdpm)
{
	/* int segs; */ 
	/* struct elf_note_info info; */
	struct user_regs_struct regs;
	/* 1 loc memory for elf header */

	/* 2 count the segment number, including note adn load segments
	 *   note segments store the thread status information
         *   load segments store the vma information
	 */
	cdpm->segments = 1 + 2;
	/* segs = cdpm-> segments; */

	get_thread_info(&regs);
	cdpm->regs	= &regs;
	/* 3 fill note segment */
	/* 3.1 fill elf header */
	fill_core_info(cdpm);

	/* 4 fill load segment */


	return 0;
}

int create_elf_core(void)
{
	FILE *fd;
	struct core_dump_params cdpm;
	int ret = 0;

	do {
		fd = fopen(CORE_FILE, "w+");
		if (!fd) {
			codp_log("create file %s fail\n", CORE_FILE);
			return -1;
		}

		cdpm.fp = fd;

		ret = elf_core_dump(&cdpm);
		if (ret != 0) {
			codp_log("write core file!@%d\n", ret);
			break;
		}

	}while(0);

	fclose(fd);

	return ret;
}
