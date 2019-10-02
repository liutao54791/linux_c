#include "c_mutex.h"

Cmutex::Cmutex():thread_self(0),m_mutex(NULL)
{
	printf("init m_mutex\n");
	thread_self = pthread_self();
	pthread_mutex_init(m_mutex, NULL);
}

Cmutex::Cmutex(pthread_mutex_t* lock)
{
	printf("init m_mutex with para\n");
	m_mutex = lock;
}

void Cmutex::lock()
{
	printf("this is thred %d lock\n", thread_self);
	pthread_mutex_lock(m_mutex);
}

void Cmutex::unlock()
{
	printf("this is thred %d unlock\n", thread_self);
	pthread_mutex_unlock(m_mutex);
}

pthread_mutex_t* Cmutex::get_mutex()
{
	printf("this is thred %d get_mutex\n", thread_self);
	return m_mutex;
}

Cmutex::~Cmutex()
{
	printf("destroy m_mutex\n");
	pthread_mutex_destroy(m_mutex);
}