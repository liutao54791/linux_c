#include "c_cond.h"

Cond::Cond()
{
	printf("init Cond\n");
	pthread_cond_init(&m_cond,NULL);
	pthread_mutex_init(&m_mutex, NULL);
}

Cond::Cond(pthread_mutex_t* mutex):m_mutex(*mutex)
{
	printf("Cond init with cond and mutex\n");
	//wait_with_mutex();
}

Cond::Cond(pthread_cond_t* cond, pthread_mutex_t* mutex):m_cond(*cond),m_mutex(*mutex)
{
	printf("Cond init with cond and mutex\n");
	//wait_with_mutex();
}

void Cond::wait()
{
	printf("Cond wait\n");
	pthread_cond_wait(&m_cond,&m_mutex);
}

void Cond::wait_with_mutex()
{
	printf("Cond wait_with_mutex\n");
	pthread_cond_wait(&m_cond,&m_mutex);
}

void Cond::wait_with_timeout(int mill)
{
	struct timespec tv;
	tv.tv_sec = mill/1000;
	tv.tv_nsec = ((mill % 1000) * 1000);

	pthread_cond_timedwait(&m_cond,&m_mutex,&tv);
}

void Cond::notify()
{
	printf("Cond notify\n");
	pthread_cond_signal(&m_cond);
}

void Cond::notifyToAll()
{
	printf("Cond notifyToAll\n");
	pthread_cond_broadcast(&m_cond);
}

pthread_cond_t* Cond::get_cond()
{
	printf("Cond get_cond\n");
	return &m_cond;
}

pthread_mutex_t* Cond::get_mutex()
{
	printf("Cond get_mutex\n");
	return &m_mutex;
}

Cond::~Cond()
{
	printf("Cond ~Cond\n");
}