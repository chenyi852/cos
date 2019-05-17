#include <stdio.h>

#include "types.h"
#include "mmu_el1.h"


void caculate_TCR_TxSZ(u32 width)
{
	u32 low_width = TCR_T0SZ(width);
	u32 up_width = (TCR_T1SZ(width) >> 16);
	u64 tx_width = TCR_TxSZ(width);
	u64 low_end_addr = 0;
	u64 up_start_addr = 0;

	PRN_ENTER(__func__);
	printf("width is %u\n\n", width);
	PRN_DEC32(low_width);
	PRN_DEC32(up_width);
	PRN_HEX64(tx_width);

	low_end_addr = (1UL << (64 - low_width)) - 1;
	printf("lower VA subrange : 0x%016lx - 0x%016lx\n", 0UL, low_end_addr );
	up_start_addr = ((~0ULL) - ((1UL << (64 - up_width)) - 1));
	printf("upper VA subrange : 0x%016lx - 0x%016lx\n", up_start_addr,
		0xffffffffffffffff );

}

void show_kern_info(void)
{
	PRN_ENTER(__func__);
	
	PRN_HEX64(KERNEL_TEXT_START);
	/* PRN_HEX64(FREE_MEM - KERNEL_TEXT_START); 
	PRN_HEX64(FREE_QM_MEM - KERNEL_TEXT_START); */
	PRN_HEX64(VA_DEV_BOOT_START);
}


void test_mmu_el1(void)
{	
	caculate_TCR_TxSZ(48);
	caculate_TCR_TxSZ(39);
	show_kern_info();
}
