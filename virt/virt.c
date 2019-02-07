#include "types.h"
#include "virt.h"

static void virt_test(void)
{
	printf("[%s]---welcome to virt world!\n", __func__);
}


int virt_main(void)
{
#ifdef CONFIG_VIRT
	virt_test();
#endif
	return 0;
}
