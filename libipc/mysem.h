#ifndef  MYSEM_H
#define MYSEM_H

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

#include <sys/sem.h>

void release_sem(int semid,int WhitchSemToRelease);

/*
 try to get sem
 WhitchSemToGet : The sem to get in the group
 */
void get_sem(int semid, int WhitchSemToGet);

/*
  function:creat a group of sems
  return value is semid !
  nsem : The num sem to creat !
 */
int open_sem(key_t key, int nsem);

/*set sem value*/
bool ctrl_sem(int semid,int WhitchSemToSet,int tosetval);

#endif