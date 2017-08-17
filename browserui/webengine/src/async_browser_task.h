#ifndef __WEBENGINE_ASYNC_BROWSER_TASK_H__
#define __WEBENGINE_ASYNC_BROWSER_TASK_H__

#include <memory>
#include <string>
#include "base/base_config.h"
#include "browser_component.h"

namespace hisense {



class AsyncBrowserTask :public BrowserComponent
{

public:
    static std::shared_ptr<AsyncBrowserTask> create( BrowserImpl*context);
    static std::shared_ptr<AsyncBrowserTask> create( BrowserImpl* context,std::string command);
    virtual ~AsyncBrowserTask();
    virtual void doWork();
protected:
   
    void setCommand(std::string cmd);
    std::string getCommand();
    AsyncBrowserTask(BrowserImpl*);
private:
    std::string m_command;

   DISALLOW_COPY_AND_ASSIGN(AsyncBrowserTask);
};

} //namespace hisense
#endif
