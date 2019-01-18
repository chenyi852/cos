#include "types.h"
#include "algorithm.h"
#include "prod_cons.h"

int algorithm_main(void)
{
	int ret = 0;
#ifdef PROCON
	ret = test_prod_cons();
#endif

	return ret;
}
