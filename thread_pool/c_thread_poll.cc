#include "c_thread_poll.h"

c_thread_poll::c_thread_poll()
{
    //thread_index[10] = {0};
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
    m_thread_poll.clear();
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
    m_thread_poll.clear();
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
        printf("m_thread_poll is clean\n");
    }
}

c_thread* c_thread_poll::get_idle_thread(void)
{
    printf("this is c_thread_poll::get_idle_thread\n");
    for (int i = 0; i < m_thread_poll.size(); i++)
    {
        if ((m_thread_poll[i] != NULL) && (m_thread_poll[i]->getIdleFlag()))
        {
            printf("this thread is idle,will break\n");
            return m_thread_poll[i];
        }
    }

}