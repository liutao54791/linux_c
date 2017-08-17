#ifndef _BROWSER_STORAGE_
#define _BROWSER_STORAGE_

#include "browser_type.h"

#include "browser_component.h"

class BrowserStorage : public BrowserComponent
{
public:
    BrowserStorage(BrowserImpl* browserImpl);
    ~BrowserStorage();

    BrowserStatus start();
    BrowserStatus stop();

private:

};

#endif
