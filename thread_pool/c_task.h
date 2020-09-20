#ifndef C_TASK_H
#define C_TASK_H

#include <sys/time.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

class c_task
{
public:
	c_task();
	void process_task();
	~c_task();
};


#endif