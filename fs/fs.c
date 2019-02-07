#include "fs.h"
#include "mangofs.h"

int fs_main(void)
{
	int ret = 0;

	fs_info("this fs---@%d\n", __LINE__);
	ret = mangofs_main(0, 0);

	return ret;
}
