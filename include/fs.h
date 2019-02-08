#ifndef _COS_FS_H
#define _COS_FS_H

#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "log.h"

#define fs_info(fmt, ...) \
	mg_info(fmt, ##__VA_ARGS__)

typedef uint32_t foff_t;

typedef struct file_descriptor{
	uint32_t inum;

}fd_t;


#ifdef CONFIG_FS
int fs_main(void);
#else
inline fs_main(void)
{
}
#endif
#endif
