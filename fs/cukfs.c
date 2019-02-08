#include "fs.h"
#include "cukfs.h"

/*******************************
 * cukfs : all the content are written in the chunk
 * *****************************
 */
struct cukfs_block{
	size_t offset;
};

struct cukfs_super{
	uint32_t block_size;
	uint32_t nblocks;

};

struct cukfs_block_data{
	char *data[CUKFS_BLOCK_SIZE];
};


struct cukfs {
	char *addr;
	struct cukfs_super super;		
	struct cukfs_inode inode;
	char* data[];
};

static struct cukfs *cukfs;
static int cukfs_mkfs(uint32_t size)
{
	cukfs->super.block_size = CUKFS_BLOCK_SIZE;
	cukfs->super.nblocks = size / CUKFS_BLOCK_SIZE;
	cukfs->inode.blocks[0] = 0;
}

static  int cukfs_init(size_t size)
{
	void *p  = malloc(size);
	if (p == NULL)
		return -1;
	cukfs = (struct cukfs *)p;
	cukfs->addr = p;

	cukfs_mkfs(size);

	return 0;
}

static fd_t *cukfs_open(uint32_t num)
{
	fd_t *fd;

	if (num == 0)
		return NULL;

	fd = malloc(sizeof(fd_t));
	if (fd == NULL)
		return NULL;
	
	fd->inum = num;
	cukfs->inode.blocks[num]=num;

	return fd;
}

static int cukfs_close(fd_t *fd)
{
	free(fd);

	return 0;
}


static size_t cukfs_write(fd_t *fd, size_t offset, void *buf, size_t size)
{
	void *fbuf;
       	foff_t foff;
		
	foff = cukfs->inode.blocks[fd->inum] * cukfs->super.block_size;
	fbuf = cukfs->addr + foff;

	memcpy(fbuf, buf, size);

	return size;
}

static size_t cukfs_read(fd_t *fd, size_t offset, void *buf, size_t size)
{
	void *fbuf;
       	foff_t foff;
		
	foff = cukfs->inode.blocks[fd->inum] * cukfs->super.block_size;
	fbuf = cukfs->addr + foff;

	memcpy(buf, fbuf, size);

	return size;
}

int cukfs_test(void)
{
	fd_t *fd;
	int tst_offset = 0x10;
	char *wrbuf="chenyi is good\n";
	char *rdbuf=malloc(strlen(wrbuf) + 1);
	
	cukfs_init(0x10000000);
	
	fd = cukfs_open(2);

	cukfs_write(fd, tst_offset, wrbuf, strlen(wrbuf) + 1);
	cukfs_read(fd, tst_offset, rdbuf, strlen(wrbuf) + 1);
	fs_info("%s", rdbuf);
	cukfs_close(fd);

	free(rdbuf);
}

int cukfs_main(int argc, char **argv)
{
	cukfs_test();

	return 0;
}
