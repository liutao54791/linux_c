#ifndef BROWSER_MODE_OFF_H
#define BROWSER_MODE_OFF_H

#include "browser-mode.h"

class BrowserModeOFF : public BrowserMode
{
public:
    BrowserModeOFF(BrowserImpl* browserImpl);

    virtual ~BrowserModeOFF();

    virtual BrowserStatus start();
    virtual void stop();
    virtual void enter(MultiModeArgument *argument = NULL);
    virtual void leave();

    virtual void setUrl(std::string url);

    virtual void processOOM() { }
private:

};

#endif