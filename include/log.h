#ifndef _MG_LOG_H
#define _MG_LOG_H

#include <stdio.h>
#include <stdarg.h>

enum mglog_level {
	MGLOG_DEBUG,
	MGLOG_INFO,
	MGLOG_WARN,
	MGLOG_ERR,
	MGLOG_CRIT,
	MGLOG_MAX,
};

extern void __attribute__((format(printf, 5, 6)))
__mglog_printf(enum mglog_level, const char *file, const char *func,
              int line, const char *fmt, ...);


#define PRN_DEC32(x) \
	printf("[%s]%-32s = %u\n", __func__, #x, x)
#define PRN_DECS32(x) \
	printf("[%s]%-32s = %d\n", __func__, #x, x)

#define PRN_HEX64(x) \
	printf("[%s]%-32s = 0x%016lx\n", __func__, #x, x)
	

#define MSTAR \
	"**************"

#define PRN_ENTER(x) \
	printf("[%s]%s %s %s\n", __func__, MSTAR, x, MSTAR);

#define PRN_EXIT(x) PRN_ENTER(x)



#define __mglog(lv, fmt...)   __mglog_printf(lv, __FILE__, __func__, __LINE__, fmt)
#define mg_debug(fmt...)      __mglog(MGLOG_DEBUG, fmt)
#define mg_info(fmt...)       __mglog(MGLOG_INFO, fmt)
#define mg_warn(fmt...)       __mglog(MGLOG_WARN, fmt)
#define mg_err(fmt...)      __mglog(MGLOG_ERR, fmt)
#define mg_crit(fmt...)      __mglog(MGLOG_CRIT, fmt)

#ifdef CONFIG_MGLOG_LEVEL
#define MGLOG_LEVEL_DEFAULT CONFIG_MGLOG_LEVEL
#else
#define MGLOG_LEVEL_DEFAULT MGLOG_INFO
#endif

#ifndef MGLOG_LEVEL
#define MGLOG_LEVEL MGLOG_LEVEL_DEFAULT
#endif
#endif
