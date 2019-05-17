#include "fs.h"
#include "cukfs.h"
#include "mangofs.h"

int fs_main(void)
{
	int ret = 0;

	fs_info("this fs---@%d\n", __LINE__);
	ret = cukfs_main(0, 0);
	ret |= mangofs_main(0, 0);

	return ret;
}
