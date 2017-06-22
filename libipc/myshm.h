#ifndef  MYSHM_H
#define MYSHM_H

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/ipc.h>

#include <sys/shm.h>

int get_shm(key_t  key, int size,int shmid);

bool read_shm(int shmid, char* readbuf);

bool write_shm(int shmid, char* writebuf);

#endif