
#include "async_browser_task.h"
#include "base/log/browser_log.h"
#include "browser-impl.h"
//FIXME:Currently we don't have context for our browser tasks.
 BrowserImpl&   browserInstance();

namespace hisense {

    std::shared_ptr<AsyncBrowserTask> AsyncBrowserTask::create(BrowserImpl* browser)
    {
        TRACE_HERE();

        std::shared_ptr<AsyncBrowserTask> task(new AsyncBrowserTask(browser));
        return task;
    }

    std::shared_ptr<AsyncBrowserTask> AsyncBrowserTask::create(BrowserImpl* browser,std::string command)
    {
        TRACE_HERE();


        std::shared_ptr<AsyncBrowserTask> task(new AsyncBrowserTask(browser));

        task->setCommand(command);

        return task;
    }

    AsyncBrowserTask::AsyncBrowserTask(BrowserImpl*browser)
        :BrowserComponent(browser)
    {

    }

    AsyncBrowserTask::~AsyncBrowserTask()
    {

    }
    /**
    * Do the actural work for the task, if the work need very long time to perform,
    * it must be run in a background thread.
    */
    void AsyncBrowserTask::doWork()
    {
        //FIXME: Currently we only have "pause" and "resume" so no need to seperate the works:
        std::string command = getCommand();
        if(command == "pause"){
            BLOGI("Running Pause Task.");
           browser().pause();

        }else if (command == "resume"){
            BLOGI("Running Pause Task.");
             browser().resume();
        }else{
             BLOGE("Running Unknown Task. %s", command.c_str());
        }

    }

    void AsyncBrowserTask::setCommand(std::string cmd)
    {
        m_command = cmd;
    }

    std::string AsyncBrowserTask::getCommand()
    {
        return m_command;
    }
}//namespace hisense
