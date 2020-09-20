#include "c_cond.h"

Cond::Cond()
{
	printf("init Cond\n");
	pthread_cond_init(&m_self_cond,NULL);
}

Cond::Cond(Cmutex* mutex):m_mutex(mutex)
{
	pthread_cond_init(&m_self_cond,NULL);
	printf("Cond init with cond and mutex\n");
	//wait_with_mutex();
}

Cond::Cond(pthread_cond_t* cond, Cmutex* mutex):m_cond(cond),m_mutex(mutex)
{
	printf("Cond init with cond and mutex\n");
	//wait_with_mutex();
}

void Cond::wait()
{
	printf("Cond wait\n");
	pthread_cond_wait(&m_self_cond,NULL);
}

void Cond::wait_with_mutex()
{
	printf("Cond wait_with_mutex\n");
	pthread_cond_wait(&m_self_cond,m_mutex->get_mutex());
}

void Cond::wait_with_timeout(int mill)
{
	struct timespec tv;
	tv.tv_sec = mill/1000;
	tv.tv_nsec = ((mill % 1000) * 1000);

	pthread_cond_timedwait(&m_self_cond,m_mutex->get_mutex(),&tv);
}

void Cond::notify()
{
	printf("Cond notify\n");
	pthread_cond_signal(&m_self_cond);
}

void Cond::notifyToAll()
{
	printf("Cond notifyToAll\n");
	pthread_cond_broadcast(&m_self_cond);
}

pthread_cond_t* Cond::get_cond()
{
	printf("Cond get_cond\n");
	return &m_self_cond;
}

Cmutex* Cond::get_mutex()
{
	printf("Cond get_mutex\n");
	return m_mutex;
}

Cond::~Cond()
{	
	pthread_cond_destroy(&m_self_cond);
	delete m_mutex;
	printf("Cond ~Cond\n");
}