#ifndef C_THREAD_H
#define C_THREAD_H

#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "c_mutex.h"
#include "c_Pthread.h"
#include "c_cond.h"
#include "c_task.h"

class c_thread:public c_Pthread
{
public:
    c_thread(/* args */);
    virtual ~c_thread();

    static void* run_thread_routing(void* object);
    virtual void start(pthread_t& thread_id);

    pthread_mutex_t* getMutexPtr();
    pthread_cond_t* getCondPtr();

    void setIdleFlag(bool flag);
    bool getIdleFlag();

    Cmutex& getMutex();
    Cond& getCond();

    c_task* task;

private:
    Cmutex* pth_mutex;
    Cond* pth_cond;
    bool isIdle;

};

#endif