#include "c_thread_manager.h"

c_thread_manager::c_thread_manager()
{
}

c_thread_manager::~c_thread_manager()
{
}

void c_thread_manager::start()
{
    printf("this is c_thread_manager::start\n");
    while (1)
    {   get_event();
        printf("get idle thread and process the event\n");
        vector<c_thread*>::iterator thread_obj_iter;
        m_poll->get_idle_thread(thread_obj_iter);
        (*thread_obj_iter)->setIdleFlag(false);
        //(*thread_obj_iter)->task = task;
        (*thread_obj_iter)->getCond().notify();
        eventQmutex->unlock();
    }
    
}

void c_thread_manager::send_event_to_queue()
{
    eventQmutex->lock();
    printf("this is c_thread_manager::send_event_to_queue\n");
    // task_list.insert(task);
    eventQmutex->unlock();
    evenCond->notify();
}

void c_thread_manager::get_event()
{
    printf("this is c_thread_manager::get_event\n");
    eventQmutex->lock();
    evenCond->wait_with_mutex();
    //task_list.front()
    //task_list.pop_front();
    eventQmutex->unlock();
    printf("get_event\n");
}

void c_thread_manager::initCondAndMutex()
{
    printf("this is c_thread_manager::initCondAndMutex\n");
    eventQmutex = new Cmutex;
    evenCond = new Cond(eventQmutex->get_mutex());
}

void c_thread_manager::init()
{
    printf("this is c_thread_manager::init\n");
    m_poll = c_thread_poll::get_instance();
    m_poll->creat_thread();
    initCondAndMutex();
}