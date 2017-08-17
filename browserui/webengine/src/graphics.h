#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "browser_type.h"

#if defined(USE_DIRECTFB)
#include <opera/components/directfb/directfb_utils.h>
#endif  // defined(USE_DIRECTFB)

#include "browser_component.h"

class Graphics : public BrowserComponent
{
public:
    Graphics(BrowserImpl* browserImpl);
    ~Graphics();

    BrowserStatus start();
    BrowserStatus stop();

    BrowserStatus requestFocus(bool request = true, bool visible = false);

    BrowserStatus lostFocus();

    opera::StaticConfiguration& getOperaConfiguration();

    int getWidth()
    {
        return m_width;
    }

    int getHeight()
    {
        return m_height;
    }

    IDirectFBDisplayLayer* getLayer()
    {
        return m_dfb_display.layer();
    }

    IDirectFB* getDFB()
    {
        return m_dfb_instance.directfb();
    }

private:

    opera::StaticConfiguration m_config;

    int m_width;
    int m_height;

    IDirectFBWindow *m_Window;

#if defined(USE_DIRECTFB)
    opera::components::DirectFBInstance m_dfb_instance;
    opera::components::DirectFBDisplay m_dfb_display;
#endif
};

#endif
