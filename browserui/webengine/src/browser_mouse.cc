#include "browser_mouse.h"
#include "browser-impl.h"
#include "graphics.h"
#include "configuration.h"
#include "directfb_debug.h"
#include "browser_debug.h"
#include "browser-input.h"
#define DEFALUT_SCREEN_WIDTH 1920
#define DEFALUT_SCREEN_HEIGHT 1080

BrowserMouse::BrowserMouse(BrowserImpl* browserImpl)
: BrowserComponent(browserImpl)
, m_dfb(NULL)
, m_layer(NULL)
, m_surface(NULL)
, m_x(0)
, m_y(0)
, m_screenWidth(DEFALUT_SCREEN_WIDTH)
, m_screenHeight(DEFALUT_SCREEN_HEIGHT)
, m_shown(false)
, m_enabled(false)
{
    BROWSER_LOG(INFO, "");
}

BrowserMouse::~BrowserMouse()
{
    BROWSER_LOG(INFO, "");
}

BrowserStatus BrowserMouse::start()
{
    BROWSER_LOG(INFO, "");
    m_dfb = browser().getGraphics().getDFB();
    m_layer = browser().getGraphics().getLayer();
    if (!m_dfb || !m_layer) {
        BROWSER_LOG(ERROR, "Failed to get resouce of DFB");
        return BrowserError;
    }

    if (initDFBMouseImage() != BrowserOK) {
        return BrowserError;
    }

    return BrowserOK;
}

void BrowserMouse::stop()
{
    BROWSER_LOG(INFO, "");
    if (m_surface) {
        DFBCHECK(m_surface->Release(m_surface));
        m_surface = NULL;
    }
}

BrowserStatus BrowserMouse::initDFBMouseImage()
{
    std::string cursor_url = browser().getConfiguration().getValueByKey("MouseUrl");
    if (!cursor_url.empty()) {
        //Create mouse surface of mouse image
        createSurface(cursor_url.c_str());
        if (!m_surface) {
            BROWSER_LOG(ERROR, "Failed to get surface of mouse image");
            return BrowserError;
        }
        DFBCHECK(m_layer->SetCursorShape(m_layer, m_surface, 0, 0));
        DFBCHECK(m_surface->Release(m_surface));
    }

    enableMouse(true);
    showMouse(false);
    setCoordinate(m_screenWidth / 2, m_screenHeight / 2);

    return BrowserOK;
}

BrowserStatus BrowserMouse::createSurface(const char *url)
{
    DFBSurfaceDescription desc;
    IDirectFBImageProvider *provider = NULL;
    if (!m_dfb) {
        return BrowserError;
    }
    DFBCHECK(m_dfb->CreateImageProvider(m_dfb, url, &provider));
    DFBCHECK(provider->GetSurfaceDescription(provider, &desc));

    desc.flags = (DFBSurfaceDescriptionFlags) (desc.flags | DSDESC_CAPS | DSDESC_PIXELFORMAT);
    desc.caps = DSCAPS_VIDEOONLY;
    desc.pixelformat = DSPF_ARGB;

    DFBCHECK(m_dfb->CreateSurface(m_dfb, &desc, &m_surface));
    DFBCHECK(provider->RenderTo(provider, m_surface, NULL));
    provider->Release(provider);

    return BrowserOK;
}

void BrowserMouse::getCoordinate(int *x, int *y)
{

    BrowserInput & input = browser().getBrowserInput();
    input.getMousePositon(&m_x,&m_y);

    *x = m_x;
    *y = m_y;
}

void BrowserMouse::setCoordinate(int x, int y)
{
    m_x = x;
    m_y = y;

    BrowserInput & input = browser().getBrowserInput();
    input.resetMousePositon(x,y);

    if (!m_layer) {
        return;
    }
    DFBCHECK(m_layer->WarpCursor(m_layer, m_x, m_y));
}

void BrowserMouse::showMouse(bool show)
{
    if (show && !m_shown) {
        if (m_layer) {
            DFBCHECK(m_layer->SetCursorOpacity(m_layer, 0xFF));
        }
        m_shown = true;
    } else if (!show && m_shown) {
        if (m_layer) {
            DFBCHECK(m_layer->SetCursorOpacity(m_layer, 0x00));
        }
        m_shown = false;
    }
}

void BrowserMouse::enableMouse(bool enable)
{
    if (enable && !m_enabled) {
        if (m_layer) {
            DFBCHECK(m_layer->EnableCursor(m_layer, enable));
        }
        m_enabled = true;
    } else if (!enable && m_enabled) {
        if (m_layer) {
            DFBCHECK(m_layer->EnableCursor(m_layer, enable));
        }
        m_enabled = false;
    }
}
