#ifndef _COS_ARM_H
#define _COS_ARM_H

#ifdef ARM
struct pt_regs {
	long uregs[18];
};

#define ARM_cpsr	uregs[16]
#define ARM_pc		uregs[15]
#define ARM_lr		uregs[14]
#define ARM_sp		uregs[13]
#define ARM_ip		uregs[12]
#define ARM_fp		uregs[11]
#define ARM_r10		uregs[10]
#define ARM_r9		uregs[9]
#define ARM_r8		uregs[8]
#define ARM_r7		uregs[7]
#define ARM_r6		uregs[6]
#define ARM_r5		uregs[5]
#define ARM_r4		uregs[4]
#define ARM_r3		uregs[3]
#define ARM_r2		uregs[2]
#define ARM_r1		uregs[1]
#define ARM_r0		uregs[0]
#define ARM_ORIG_r0	uregs[17]

/* --- arch/arm/include/asm/elf.h --- */
typedef unsigned long elf_greg_t;
typedef unsigned long elf_freg_t[3];

#define ELF_NGREG (sizeof (struct pt_regs) / sizeof(elf_greg_t))

typedef elf_greg_t elf_gregset_t[ELF_NGREG];

typedef struct user_fp elf_fpregset_t;

/* -- arch/arm/include/uapi/asm/posix_types.h -- */
typedef unsigned short		__kernel_mode_t;
#define __kernel_mode_t __kernel_mode_t

typedef unsigned short		__kernel_ipc_pid_t;
#define __kernel_ipc_pid_t __kernel_ipc_pid_t

typedef unsigned short		__kernel_uid_t;
typedef unsigned short		__kernel_gid_t;
#define __kernel_uid_t __kernel_uid_t

typedef unsigned short		__kernel_old_dev_t;
#define __kernel_old_dev_t __kernel_old_dev_t

#endif /* ARM */

#endif
