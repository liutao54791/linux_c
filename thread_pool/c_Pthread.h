#ifndef C_PTHREAD_H
#define C_PTHREAD_H

#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

class c_Pthread
{
private:
    /* data */
public:
    c_Pthread(/* args */)
    {};
    virtual ~c_Pthread()
    {};
    virtual void start(pthread_t& thread_id) = 0;
};

#endif