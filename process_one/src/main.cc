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

#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "message.h"

void* pthread_func(void* arg)
{
    struct msg_st recdata;
    int msgid;
    if ((msgid = getMessage((key_t)1234, 0666|IPC_CREAT)) < 0)
    {
        printf("get message error\n");
    }
    while(1)
    {
        printf("this is process pthread\n");
        recMessage(msgid,(void*)&recdata, 10, 0, 0) ;
        printf("receive data %s\n",recdata.message);
        if (strncmp(recdata.message ,"end", 3) == 0)
        {
            printf("receive it \n");
        }
    }
}

int main(int argc, char* argv[])
{
    int err;
    void* res;
    printf("this is process one\n");

    pthread_t thread_id;
    if ( pthread_create(&thread_id,NULL, pthread_func, NULL) != 0)
    {
        perror("pthread create failed");
    }
    err = pthread_join(thread_id, &res);
    if (err != 0)
    {
        printf("can not join thread %d\n", strerror(err));
    }
}
