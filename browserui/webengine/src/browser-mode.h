#ifndef BROWSER_MODE_H
#define BROWSER_MODE_H

#include <string>

#include "browser_type.h"
#include "browser-impl.h"
#include "browser_component.h"
class BrowserInput;

namespace opera {
class Opera;
namespace components {
class OperaController;

namespace input {
class InputReader;
}
}
}

class BrowserMode : public hisense::BrowserComponent
{
public:

    BrowserMode(BrowserImpl* browserImpl)
    : BrowserComponent(browserImpl) { }

    virtual ~BrowserMode() { }

    virtual BrowserStatus start() = 0;
    virtual void stop() = 0;
    virtual void enter(MultiModeArgument *argument = NULL) = 0;
    virtual void leave() = 0;

    virtual void pause() { }

    virtual void resume() { }

    virtual void injectXmlAit(std::string url) { }

    virtual void setNetStatus(const int status) { }

    virtual void ReTune() { }

    virtual void setUrl(std::string url) = 0;

    virtual void processOOM() = 0;

    virtual void sendMessage(const std::string &) { }

};

#endif
