#include <stdio.h>

#include "types.h"
#include "mmu_el1.h"

/*******************************************
	for old kernel
  ***************************************/
#define BIT(n) (1ul << (n))
#define MASK(n) (BIT(n)-1ul)

#define ARM_1GB_BLOCK_BITS      30
#define ARM_2MB_BLOCK_BITS      21

#define PGDE_SIZE_BITS          3
#define PGD_BITS                9
#define PGD_SIZE_BITS           (PGD_BITS + PGDE_SIZE_BITS)

#define PUDE_SIZE_BITS          3
#define PUD_BITS                9
#define PUD_SIZE_BITS           (PUD_BITS + PUDE_SIZE_BITS)

#define PMDE_SIZE_BITS          3
#define PMD_BITS                9
#define PMD_SIZE_BITS           (PMD_BITS + PMDE_SIZE_BITS)

#define GET_PGD_INDEX(x)        (((x) >> (ARM_2MB_BLOCK_BITS + PMD_BITS + PUD_BITS)) & MASK(PGD_BITS))
#define GET_PUD_INDEX(x)        (((x) >> (ARM_2MB_BLOCK_BITS + PMD_BITS)) & MASK(PUD_BITS))
#define GET_PMD_INDEX(x)        (((x) >> (ARM_2MB_BLOCK_BITS)) & MASK(PMD_BITS))

#define PRN_PT_BITS(x)	printf("%-24s : \t\t%u\n", #x, x);

void print_old_info(void)
{
	unsigned long base_addr = 0x1234567812;

	PRN_PT_BITS(PGD_BITS);
	PRN_PT_BITS(ARM_2MB_BLOCK_BITS);
	PRN_PT_BITS(PMD_BITS);
	PRN_PT_BITS(PUD_BITS);	
	printf("\n------%s-------\n", "old kernel");
	
	printf("GPD_INDEX = %lu\n", GET_PGD_INDEX(base_addr));

	printf("\n------%s-------\n", "old kernel");
}

void va_entry(void)
{
	test_mmu_el1();
	print_old_info();

}
