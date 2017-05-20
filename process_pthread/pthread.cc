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

#include "m_pthread.h"

pthread_mutex_t   mutex;
pthread_cond_t      thread_cond;
int my_num = 0;

void *pthread_func(void * arg)
{
    //pthread_func2 run early
    //sleep(10);
    pid_t pid;

    pid = getpid();
    printf("the pid in func is %d\n", pid);

    while(1)
    {
        pthread_cond_wait(&thread_cond, &mutex);
        printf("my_num  in func is %d\n", my_num);
        my_num --;
        pthread_mutex_unlock(&mutex);
        printf("the mutex  in func is resleased\n");
    }
}

void *pthread_func2(void* arg)
{
    //ARG* p = (ARG*)arg;
    //printf("arg1 is : %s, arg2 is : %d, arg3 is : %f\n", p->arg1, p->arg2,  p->arg3); 
    pid_t pid;
    pid = getpid();
    printf("the pid in func2 is %d\n", pid);
    while(1)
    {
        sleep(1);
        pthread_mutex_lock(&mutex);
        printf("this is the thread 2\n");
        if (my_num == 0)
        {
            pthread_cond_signal(&thread_cond);
            printf("my_num  in func2 is %d\n", my_num);
            my_num ++;
            pthread_mutex_unlock(&mutex);
        }

    }
    return NULL;
}

void *pthread_func3(void* arg)
{
    pid_t pid;
    int i = 0;

    pid = getpid();
    printf("the pid is %d\n", pid);
    while(i != 10)
    {
        printf("this is the third thread\n");
        i ++;
    }
    return NULL;
}

void process_pthread(void)
{
/****************************线程实验****************************s*/
    pthread_t thread_id;
    pthread_t thread_id2;
    pthread_t thread_id3;

    pid_t pid;
    int err;
    void* res;

    pthread_mutex_init(&mutex, NULL); 
    pthread_cond_init(&thread_cond, NULL);
    pid = getpid();
    printf("pid is %d\n", pid);
    if ((err = pthread_create(&thread_id,NULL, pthread_func, NULL)) != 0)
    {
        perror("pthread create failed");
    }

    if ((err = pthread_create(&thread_id2,NULL, pthread_func2, NULL)) != 0)
    {
        perror("pthread create failed");
    }

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if ((err = pthread_create(&thread_id3,&attr, pthread_func3, NULL)) != 0)
    {
        perror("pthread create failed");
    }

    err = pthread_join(thread_id, &res);
    if (err != 0)
    {
        printf("can not join thread %d\n", strerror(err));
    }

    err = pthread_join(thread_id2, &res);
    if (err != 0)
    {
        printf("can not join thread %d\n", strerror(err));
    }
}