#ifndef _COMM_H_
#define _COMM_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PATHNAME "." // ftok函数 生成key使用
#define PROJ_ID 66 // ftok 函数生成key使用

int create_shm( int size);//  分配指定大小的共享内存块
int destroy_shm( int shmid); // 释放指定id的共享内存块
int get_shmid(); // 获取已经存在的共享内存块


#endif /*_COMM_H_*/
