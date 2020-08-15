#ifndef C_THREAD_POLL_H
#define C_THREAD_POLL_H

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

using namespace std;

class c_thread_poll
{
public:
    void creat_thread();
    void creat_threads(unsigned int num);

    void delete_all_threads();

    void get_idle_thread(vector<c_thread*>::iterator& thread_obj_iter);

    static c_thread_poll* get_instance();

private:
    c_thread_poll(/* args */);
    ~c_thread_poll();
    static c_thread_poll* m_instance;
    vector<c_thread*> m_thread_poll;
};

#endif