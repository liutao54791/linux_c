#ifndef HISENSE_BROWSER_MOUSE_H
#define HISENSE_BROWSER_MOUSE_H

#include <directfb.h>
#include "browser_type.h"

#include "browser_component.h"

class BrowserMouse : public BrowserComponent
{
public:
    BrowserMouse(BrowserImpl* browserImpl);
    ~BrowserMouse();

    BrowserStatus start();
    void stop();

    void getCoordinate(int *x, int *y);
    void setCoordinate(int x, int y);

    void showMouse(bool show);
    ;
    void enableMouse(bool enable);

private:
    BrowserStatus initDFBMouseImage();
    BrowserStatus createSurface(const char *url);

private:

    IDirectFB* m_dfb;
    IDirectFBDisplayLayer* m_layer;
    IDirectFBSurface* m_surface;

    int m_x;
    int m_y;

    int m_screenWidth;
    int m_screenHeight;

    bool m_shown;
    bool m_enabled;
};

#endif