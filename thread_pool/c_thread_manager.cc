#include "c_thread_manager.h"

c_thread_manager::c_thread_manager()
{
    c_task_list.clear();
}

c_thread_manager::~c_thread_manager()
{
    delete evenCond;
}

void* c_thread_manager::run_thread_routing(void* obj)
{
    while (1)
    {   
        c_thread_manager* manager = (c_thread_manager*)obj;
        c_task* task =  manager->get_event();
        printf("get idle thread and process the event\n");
        c_thread* thread_obj = manager->m_poll->get_idle_thread();
        thread_obj->setIdleFlag(false);
        thread_obj->task = task;
        thread_obj->getCond().notify();
    }   
}

void c_thread_manager::start()
{
    printf("this is c_thread_manager::start\n");

    int err = 0;
    pthread_t thread_id;
    if ((err = pthread_create(&thread_id, NULL, c_thread_manager::run_thread_routing, this)) != 0)
    {
        printf("c_thread_manager  pthread create failed\n");
    }
    else
    {
        printf("c_thread_manager  pthread create succesfully %d\n",thread_id);
    }

}

void c_thread_manager::send_event_to_queue(c_task* task)
{
    evenCond->get_mutex()->lock();
    printf("this is c_thread_manager::send_event_to_queue\n");
    if (task != NULL)
    {
        c_task_list.push_back(task);
    }
    evenCond->get_mutex()->unlock();
    evenCond->notify();
}

c_task* c_thread_manager::get_event()
{
    printf("this is c_thread_manager::get_event\n");
    c_task* task = NULL;
    evenCond->get_mutex()->lock();

    evenCond->wait_with_mutex();
    if (!c_task_list.empty())
    {
        task = c_task_list.front();
        c_task_list.pop_front();
    }

    evenCond->get_mutex()->unlock();
    printf("get_event successfully\n");
    return task;
}

void c_thread_manager::initCondAndMutex()
{
    printf("this is c_thread_manager::initCondAndMutex\n");
    eventQmutex = new Cmutex;
    evenCond = new Cond(eventQmutex);
}

void c_thread_manager::init()
{
    printf("this is c_thread_manager::init\n");
    m_poll = c_thread_poll::get_instance();
    m_poll->creat_thread();
    initCondAndMutex();
    //start();
}
