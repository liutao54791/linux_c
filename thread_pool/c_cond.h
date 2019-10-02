#ifndef C_COND_H
#define C_COND_H

#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

class Cond
{
public:
	Cond();
	Cond(pthread_cond_t* cond, pthread_mutex_t* mutex);

	void wait();
	void wait_with_timeout(int mill);

	void notify();
	void notifyToAll();

	~Cond();

private:
	pthread_cond_t* m_cond;
	pthread_mutex_t* m_mutex;
	
};

#endif