#include "c_cond.h"

Cond::Cond(pthread_cond_t* cond, pthread_mutex_t* mutex):m_cond(cond),m_mutex(mutex)
{
	printf("Cond init with cond and mutex\n");
	wait();
}

void Cond::wait()
{
	printf("Cond wait\n");
	pthread_cond_wait(m_cond,m_mutex);
}

void Cond::wait_with_timeout(int mill)
{
	struct timespec tv;
	tv.tv_sec = mill/1000;
	tv.tv_nsec = ((mill % 1000) * 1000);

	pthread_cond_timedwait(m_cond,m_mutex,&tv);
}

void Cond::notify()
{
	printf("Cond notify\n");
	pthread_cond_signal(m_cond);
}

void Cond::notifyToAll()
{
	printf("Cond notifyToAll\n");
	pthread_cond_broadcast(m_cond);
}

Cond::~Cond()
{
	printf("Cond ~Cond\n");
}