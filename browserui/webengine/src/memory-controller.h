#ifndef _BROWSER_MEMROY_CONTROLLER_H_
#define _BROWSER_MEMROY_CONTROLLER_H_

#include "browser_type.h"
#include "browser_component.h"

class MemroyController : public BrowserComponent
{
public:
    MemroyController(BrowserImpl* browserImpl);
    ~MemroyController();

    BrowserStatus start();
    BrowserStatus stop();

    bool memoryCheck();

private:
    void set_oom_adj();
    void drop_cache();
    void dumpMemory();

private:
    int m_dropCacheTime;
    int m_outOfMemroySize;
    int m_outOfFreeMemorySize;
};

#endif
