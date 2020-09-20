#include "c_thread.h"

c_thread::c_thread():isIdle(true),task(NULL)
{
    pth_mutex = new Cmutex;
    pth_cond = new Cond(pth_mutex);
    printf("this is c_thread::c_thread\n");
}

c_thread::~c_thread()
{
    delete pth_cond;
    printf("this is c_thread::~c_thread\n");
}

void* c_thread::run_thread_routing(void* object)
{
    printf("this is c_thread run_thread_routing and waiting for event\n");
    c_thread* m_thread = (c_thread*) object;
    while (1)
    {
        m_thread->pth_mutex->lock();
        m_thread->pth_cond->wait_with_mutex();
        m_thread->task->process_task();
        printf("this is c_thread run_thread_routing %d\n",pthread_self());
        m_thread->setIdleFlag(true);
        m_thread->pth_mutex->unlock();
    }
}

void c_thread::start(pthread_t& thread_id)
{
    printf("this is c_thread start\n");
    int err = 0;
    if ((err = pthread_create(&thread_id, NULL, c_thread::run_thread_routing, this)) != 0)
    {
        printf("pthread create failed\n");
    }
    else
    {
        printf("pthread create succesfully %d\n",thread_id);
    }
}

pthread_mutex_t* c_thread::getMutexPtr()
{
    printf("this is c_thread getMutex\n");
    return pth_mutex->get_mutex();
}

pthread_cond_t* c_thread::getCondPtr()
{
    printf("this is c_thread getCond\n");
    return pth_cond->get_cond();
}

void c_thread::setIdleFlag(bool flag)
{
    isIdle = flag;
}

bool c_thread::getIdleFlag()
{
    return isIdle;
}

Cmutex& c_thread::getMutex()
{
    printf("this is c_thread getMutex\n");
    return *pth_mutex;
}

Cond& c_thread::getCond()
{
    printf("this is c_thread getCond\n");
    return *pth_cond;
}