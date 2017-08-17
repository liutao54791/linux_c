#ifndef __WEBENGINE_BROWSER_COMPONENT_BASE_H__
#define __WEBENGINE_BROWSER_COMPONENT_BASE_H__

#include "base/log/browser_log.h"

/**
 * This class is the base class of browser components
 * Make it easier to access browserImpl and other components.
 */
class BrowserImpl;

namespace hisense {

class BrowserComponent
{
protected:

    BrowserComponent(BrowserImpl *browser)
    : m_browser(browser)
    {
        if (browser == NULL) {
            BLOGF("Invalid Browser Instance.");
            BLOGA("Invalid Browser Instance.");
        }
    }

    virtual ~BrowserComponent()
    {
        m_browser = NULL;
    }

    BrowserImpl& browser() const
    {
        return *m_browser;
    }
private:
    BrowserImpl * m_browser; //The parent browser instance

};
}//namespace hisense

using hisense::BrowserComponent;

#endif
