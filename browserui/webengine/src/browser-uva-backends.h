#ifndef _BROWSER_UVA_BACKEND_
#define _BROWSER_UVA_BACKEND_

#include "browser_type.h"

#include "browser_component.h"

class BrowserUVABackend : BrowserComponent
{
public:
    BrowserUVABackend(BrowserImpl* browserImpl);
    ~BrowserUVABackend();

    BrowserStatus start();
    BrowserStatus stop();


};

#endif
