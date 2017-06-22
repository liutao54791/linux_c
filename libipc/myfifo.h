#ifndef  MYFIFO_H
#define MYFIFO_H

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

bool creat_fifo(mode_t  mode,char* fifo_name);

bool read_fifo(char* readbuf, char* fifo_name);

bool write_fifo(char* write_string , char* fifo_name);

bool close_fifo(int fd);

#endif