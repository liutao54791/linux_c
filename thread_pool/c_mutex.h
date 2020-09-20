#ifndef C_MUTEX_H
#define C_MUTEX_H

#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

class Cmutex
{
public:
	Cmutex();
	Cmutex(pthread_mutex_t* lock);

	void lock();

	void unlock();

	pthread_mutex_t* get_mutex();
	pthread_mutex_t  init_mutex;
	pthread_mutex_t* m_mutex;

	~Cmutex();

private:
	pthread_t thread_self;
	
};

#endif