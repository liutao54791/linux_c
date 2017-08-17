
#ifndef __WEBENGINE_TASK_MANAGER_H__
#define __WEBENGINE_TASK_MANAGER_H__

#include <memory>
#include "async_browser_task.h"
#include "browser_component.h"
#include "browser_type.h"
#include "base/container/blocking_queue.h"
#include "base/threading/basic_thread.h"

class BrowserImpl;

class TaskManager : public BrowserComponent
{
public:
    TaskManager(BrowserImpl*);
    ~TaskManager();
    // BrowserStatus start();
    // BrowserStatus stop();
    bool postTask(std::shared_ptr<hisense::AsyncBrowserTask> task);
    BrowserStatus start();

private:
    void startScheduler();
    void stopScheduler();
    void scheduleTasks();
    std::shared_ptr<hisense::AsyncBrowserTask> getNextTask();
    void dispatchTask(std::shared_ptr<hisense::AsyncBrowserTask>);

    //for BasicThread:
    friend class base::Threading::BasicThread <TaskManager>;
    void* run();
    void stop();


    bool m_running;
    std::shared_ptr<base::Threading::BasicThread <TaskManager> >m_working_thread;
    base::Container::BlockingQueue<std::shared_ptr<hisense::AsyncBrowserTask> > m_tasks;

};


#endif

