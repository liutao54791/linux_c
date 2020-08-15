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

#include "c_mutex.h"
#include "c_Pthread.h"
#include "c_cond.h"
#include "c_thread.h"
#include "c_thread_poll.h"

using namespace std;

class c_thread_manager
{
public:
    c_thread_manager();
    ~c_thread_manager();

    void start();
    void send_event_to_queue();

    void initCondAndMutex();
    void init();
private:
    void get_event();
    Cond* evenCond;
    Cmutex* eventQmutex;
    c_thread_poll* m_poll;
};

#endif