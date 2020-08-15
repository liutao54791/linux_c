#include "c_thread_poll.h"

c_thread_poll::c_thread_poll()
{
    delete_all_threads();
}

c_thread_poll::~c_thread_poll()
{
    delete_all_threads();
}

c_thread_poll* c_thread_poll::m_instance = NULL;
c_thread_poll* c_thread_poll::get_instance()
{
    printf("this is c_thread_poll::get_instance\n");
    if (m_instance == NULL)
    {
        printf("creat c_thread_poll\n");
        m_instance =  new c_thread_poll();
    }
    return m_instance;

}

void c_thread_poll::creat_thread()
{
    printf("this is c_thread_poll::creat_thread\n");
    pthread_t thread_index[10];
    for (int i = 0; i < 10; i++)
    {
        c_thread* m_thread = new c_thread();
        m_thread->start(thread_index[i]);
        m_thread_poll.push_back(m_thread);
        sleep(1);
    }
}

void c_thread_poll::creat_threads(unsigned int num)
{
    printf("this is c_thread_poll::creat_threads\n");
    pthread_t thread_index[10];
    if (num > 0)
    {
        for (int i = 0; i < num; i++)
        {
            c_thread* m_thread = new c_thread();
            m_thread->start(thread_index[i]);
            m_thread_poll.push_back(m_thread);
            sleep(1);
        }
    }
    else
    {
        printf("num is < 0\n");
    }
}

void c_thread_poll::delete_all_threads()
{
    printf("this is c_thread_poll::delete_all_threads\n");

    if (m_thread_poll.size() > 0)
    {
        for (int i = 0; i < m_thread_poll.size(); i++)
        {
            delete m_thread_poll[i];
        }
        m_thread_poll.clear();
    }
    else
    {
        printf("m_thread_poll is ckean\n");
    }
}

void c_thread_poll::get_idle_thread(vector<c_thread*>::iterator& thread_obj_iter)
{
    printf("this is c_thread_poll::get_idle_thread\n");
    for (thread_obj_iter = m_thread_poll.begin(); thread_obj_iter != m_thread_poll.end(); thread_obj_iter++)
    {
        if ((*thread_obj_iter)->getIdleFlag())
        {
            break;
        }
    }

}