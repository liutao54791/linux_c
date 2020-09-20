#ifndef C_COND_H
#define C_COND_H

#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "c_mutex.h"

class Cond
{
public:
	Cond();
	Cond(Cmutex* mutex);
	Cond(pthread_cond_t* cond, Cmutex* mutex);

	void wait();
	void wait_with_mutex();
	void wait_with_timeout(int mill);

	void notify();
	void notifyToAll();

	pthread_cond_t* get_cond();
	Cmutex* get_mutex();

	~Cond();

private:
	pthread_cond_t m_self_cond;
	pthread_mutex_t m_self_mutex;

	pthread_cond_t* m_cond;
	Cmutex* m_mutex;
	
};

#endif