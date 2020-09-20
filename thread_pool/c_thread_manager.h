#ifndef C_THREAD_MANAGER_H
#define C_THREAD_MANAGER_H

#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <list>

#include "c_mutex.h"
#include "c_Pthread.h"
#include "c_cond.h"
#include "c_thread.h"
#include "c_thread_poll.h"
#include "c_task.h"

using namespace std;

class c_thread_manager
{
public:
    c_thread_manager();
    ~c_thread_manager();

    void start();
    static void* run_thread_routing(void* obj);
    void send_event_to_queue(c_task* task);

    void initCondAndMutex();
    void init();
private:
    c_task* get_event();

    std::list<c_task*> c_task_list;
    Cond* evenCond;
    Cmutex* eventQmutex;
    c_thread_poll* m_poll;
};

#endif