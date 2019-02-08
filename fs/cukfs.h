#ifndef __CUKFS_H
#define __CUKFS_H

#define CUKFS_BLOCK_SIZE	0x100000
#define CUKFS_MAX_BLOCK_NUM	128


struct cukfs_inode{
	uint32_t	blocks[CUKFS_MAX_BLOCK_NUM];

};

int cukfs_main(int argc, char **argv);

#endif
