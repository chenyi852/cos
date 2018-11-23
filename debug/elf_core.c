#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "elf_core.h"

typedef struct i386_regs {    /* Normal (non-FPU) CPU registers            */
    #define BP rbp
    #define SP rsp
    #define IP rip
    u64  r15,r14,r13,r12,rbp,rbx,r11,r10;
    u64  r9,r8,rax,rcx,rdx,rsi,rdi,orig_rax;
    u64  rip,cs,eflags;
    u64  rsp,ss;
    u64  fs_base, gs_base;
    u64  ds,es,fs,gs;
}regs;

#include <sys/reg.h>

int get_thread_info(void)
{
	char scratch[4096];
	pid_t child;
	//pid_t pid = getpid();
	regs thread_regs;
	/* save the instruction to be executred next */
	long inst;
	long orig_rax;
	long rip;
	child = fork();
	if (child == 0) {
		ptrace(PTRACE_TRACEME,0,NULL,NULL);
		execl("/bin/ls", "ls",NULL);
	}
	else {

		wait(NULL);
		orig_rax = ptrace(PTRACE_PEEKUSER,child,8*ORIG_RAX,NULL);
		printf("The child made a system call %ld\n",orig_rax);
		rip = ptrace(PTRACE_PEEKUSER, child, 8 * RIP,NULL);
		printf("The RIP is  %lx\n", rip);

		memset(scratch, 0xFF, sizeof(scratch));
		inst  =ptrace(PTRACE_PEEKTEXT, child, rip, NULL);
		printf("tracee:RIP:0x%lx INST: 0x%lx\n", rip, inst);
#if 0
		if (ptrace(PTRACE_ATTACH, child, (void *)0,
					(void *)0) < 0) {
			printf("enter the debug for adam %s %d\n", __FUNCTION__, __LINE__);
		}
#endif
		if (ptrace(PTRACE_GETREGS, child, scratch, scratch) == 0) {
			memcpy(&thread_regs, scratch, sizeof(regs));
			printf("rip = %lx\n", thread_regs.rip);
			printf("rsp = %lx\n", thread_regs.rsp);
			printf("orig_rax = %lu\n", thread_regs.orig_rax);
		}
		ptrace(PTRACE_CONT,child,NULL,NULL);

	}

}
