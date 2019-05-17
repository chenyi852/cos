#include "log.h"


#define LOG_BUF_SIZE 256

static char log_buf[LOG_BUF_SIZE];

static const char *log_level_str[] = {
	[MGLOG_DEBUG]	= "DEBUG",
	[MGLOG_INFO] 	= "INFO",
	[MGLOG_WARN]	= "WARNING",
	[MGLOG_ERR]	= "ERROR",
	[MGLOG_CRIT]	= "CRITICAL",
};

void  __attribute__((format(printf, 5, 6)))
__mglog_printf(enum mglog_level level, const char *file, const char *func,
              int line, const char *fmt, ...)
{
	va_list args;
	int len = 0;
	if (level < MGLOG_LEVEL)
		return;
	if (level >= MGLOG_MAX)
		level = MGLOG_MAX - 1;

	va_start(args, fmt);

	len = snprintf(log_buf, (size_t)(LOG_BUF_SIZE - 1), "[%s]@(%s:%d): ", log_level_str[level], func, line);
	if (len >= 0 && len < (int)LOG_BUF_SIZE) {
		int ret;
		ret = vsnprintf(&log_buf[len], (size_t)(LOG_BUF_SIZE - len - 1), fmt, args);
		if (ret < 0) {
			va_end(args);
			return;
		}
	} else {
		va_end(args);
		return;
	}
	log_buf[LOG_BUF_SIZE - 1] = '\0';
	printf("%s", log_buf);
	va_end(args);
}

