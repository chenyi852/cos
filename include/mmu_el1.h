#ifndef _MMU_EL1_H
#define _MMU_EL1_H

#include "log.h"
#include "types.h"

/* Virtual address layout
 *
 * VA_START
 * |
 * V
 * |<---------------------------  vaddr space ------------------------------------------------------------------------------>|
 * |<- 1/4 ->|<- 1/4 -->|<-- 1/4 -->|
 *                                  |<- TEXT_OFFSET ->|<- KERNEL TEXT ->|<- RODATA ->|<- DATA ->|<- FREE MEM ->|  |<-ERRPTR->|
 *           |<-DEVICE->|    |<------------------ 1:1 mapping -----(main bank memory)------------------------->|
 *
 * breakdown DEVICE:
 *
 * |<--------------------- DEVICE -------------------------------->|
 * |<------ 1/4 ------>|<-------- 1/4 -------> |<--1/4-->|<--1/4-->|
 * |<- kernel driver ->|                       |<--BOOT->|<--GAP-->|
 */
#define CONFIG_AARCH64_TEXT_OFFSET 	(0x200000UL)
#define CONFIG_AARCH64_VA_BITS (39)

#define TEXT_OFFSET CONFIG_AARCH64_TEXT_OFFSET
#define VA_BITS CONFIG_AARCH64_VA_BITS

#define VA_START ((0xffffffffffffffffUL) - ((1UL) << VA_BITS) + (1UL))
#define VA_ID_MAP_START	((0xffffffffffffffffUL) - ((1UL) << (VA_BITS - 2)) + (1UL))

#define VA_DEV_START		(VA_START + ((1UL) << (VA_BITS - 2)))

#define VA_DEV_KDEV_START	VA_DEV_START
#define VA_DEV_KDEV_END		(VA_DEV_START + ((1UL) << (VA_BITS - 4)))

#define VA_DEV_BOOT_START	(VA_DEV_START + ((1UL) << (VA_BITS - 3)))

#define KERNEL_TEXT_START (VA_ID_MAP_START + (TEXT_OFFSET))


/* TCR flags */
#define TCR_T0SZ(x)	((64UL) - x)
#define TCR_T1SZ(x)	(((64UL) - x) << 16)
#define TCR_TxSZ(x)	(TCR_T0SZ(x) | TCR_T1SZ(x))

extern void caculate_TCR_TxSZ(u32 width);

extern void show_kern_info(void);

extern void test_mmu_el1(void);
#endif
