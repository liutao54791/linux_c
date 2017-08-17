

#include "task_manager.h"
#include "browser-impl.h"
#include "boost/bind.hpp"

TaskManager::TaskManager(BrowserImpl*browser)
: BrowserComponent(browser)
, m_running(true)

{
    TRACE_HERE();
}

TaskManager::~TaskManager()
{
    TRACE_HERE();
}

BrowserStatus TaskManager::start()
{
    startScheduler();
    return BrowserOK;
}

bool TaskManager::postTask(std::shared_ptr<hisense::AsyncBrowserTask> task)
{
    TRACE_HERE();

    m_tasks.enqueue(task);

    return true;
}

std::shared_ptr<hisense::AsyncBrowserTask> TaskManager::getNextTask()
{
    //The Queue is a Blocking Queue, will wait here when empty:
    return m_tasks.dequeue();

}

/**
 * Schedule task on main thread.
 */
void TaskManager::scheduleTasks()
{
    //TODO: scheduleTasks based on delay interval of tasks:

}

void TaskManager::startScheduler()
{
    //TODO:Using a timer to schedule the tasks:
    //Currently we are using a thread to simulate this.
    m_working_thread.reset(new base::Threading::BasicThread<TaskManager>(*this));

}

void TaskManager::stopScheduler()
{
    //TODO:Using a timer to schedule the tasks:
    m_running = false;

    //Will call Stop if destructed:
    m_working_thread.reset();
}

/**
 * Dispatch task into background thread.
 */
void TaskManager::dispatchTask(std::shared_ptr<hisense::AsyncBrowserTask> task)
{
    //TODO: dispatch the task to a background thread.
    //Currently we just do it now.
    if (task) {
        task->doWork();
    }
}
//Taskmanager thread to schedule the tasks.

void* TaskManager::run()
{
    while (m_running) {
        std::shared_ptr<hisense::AsyncBrowserTask> task = getNextTask();
        if (task) {
            //FIXME: This function must be return immediately:
            dispatchTask(task);
        } else {
            //Exit thread:
            BLOGI("Exiting TaskManager...");
            break;
        }
    }
    return NULL;
}

void TaskManager::stop()
{
    m_running = false;
    std::shared_ptr<hisense::AsyncBrowserTask> exit_task;
    postTask(exit_task);
}


