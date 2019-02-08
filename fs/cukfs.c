#include "fs.h"

/*******************************
 * cukfs : all the content are written in the chunk
 * *****************************
 */
struct cukfs {
	char *addr;
	char* data[];
};
static struct cukfs *cukfs;
static  int cukfs_init(size_t size)
{
	void *p  = malloc(size);
	if (p == NULL)
		return -1;
	cukfs = (struct cukfs *)p;
	cukfs->addr = p;

	return 0;
}

static size_t cukfs_write(size_t offset, void *buf, size_t size)
{
	void *fbuf = &cukfs->data[offset];

	memcpy(fbuf, buf, size);

	return size;
}

static size_t cukfs_read(size_t offset, void *buf, size_t size)
{
	void *fbuf = &cukfs->data[offset];

	memcpy(buf, fbuf, size);

	return size;
}

int cukfs_test(void)
{
	int tst_offset = 0x10;
	char *wrbuf="chenyi is good\n";
	char *rdbuf=malloc(strlen(wrbuf) + 1);

	cukfs_init(0x100000);

	cukfs_write(tst_offset, wrbuf, strlen(wrbuf) + 1);
	cukfs_read(tst_offset, rdbuf, strlen(wrbuf) + 1);
	fs_info("%s", rdbuf);

}

int cukfs_main(int argc, char **argv)
{
	cukfs_test();

	return 0;
}
