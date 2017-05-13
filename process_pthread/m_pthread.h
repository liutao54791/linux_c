#ifndef  M_PTHEREAD_H
#define M_PTHEREAD_H

typedef struct arg_struct ARG;

struct  arg_struct
{
    char   arg1[10];
    int      arg2;
    float   arg3;
};

void *pthread_func2(void* arg);

void *pthread_func(void * arg);

void *pthread_func3(void* arg);

void process_pthread(void);

#endif

