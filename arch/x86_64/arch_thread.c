#include <sys/types.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "arch_elf.h"
#include "arch_thread.h"




int get_thread_info(struct user_regs_struct *regs)
{
	char scratch[4096];
	pid_t child;
	//pid_t pid = getpid();
	pt_regs *thread_regs = regs;
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
		printf("The RIP is  %llx\n", rip);

		memset(scratch, 0xFF, sizeof(scratch));
		inst  =ptrace(PTRACE_PEEKTEXT, child, rip, NULL);
		printf("tracee:RIP:0x%llx INST: 0x%llx\n", rip, inst);
#if 0
		if (ptrace(PTRACE_ATTACH, child, (void *)0,
					(void *)0) < 0) {
			printf("enter the debug for adam %s %d\n", __FUNCTION__, __LINE__);
		}
#endif
		if (ptrace(PTRACE_GETREGS, child, scratch, scratch) == 0) {
			memcpy(thread_regs, scratch, sizeof(regs));
			printf("rip = %llx\n", thread_regs->rip);
			printf("rsp = %llx\n", thread_regs->rsp);
			printf("orig_rax = %lu\n", thread_regs->orig_rax);
		}
		ptrace(PTRACE_CONT,child,NULL,NULL);

	}

}
