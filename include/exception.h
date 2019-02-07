#ifndef _MG_EXCEPTION_H
#define _MG_EXCEPTION_H

static inline void __attribute__((noreturn)) __os_abort(void)
{
	__builtin_trap();
	__builtin_unreachable();
}

#endif
