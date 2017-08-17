#include "browser_debug.h"
#include "browser-uva-backends.h"
#include "graphics.h"
#include "browser-impl.h"
#include "NotImplemented.h"
#if 0 // can't use mtk stuff here:
#ifdef USE_MTK_NEW_BACKENDS
#include "uva/backends/mtk/backend/IPluginMgr.h"
#else
extern "C"
{
    void vPiMgr_UvaInit(int width, int height, char* strUA);
    int vPiMgr_Uninit();
}
#endif
#endif

BrowserUVABackend::BrowserUVABackend( BrowserImpl* browserImpl )
    : BrowserComponent( browserImpl )
{
    BROWSER_LOG( INFO, "" );
}

BrowserUVABackend::~BrowserUVABackend()
{
    BROWSER_LOG( INFO, "" );
}

BrowserStatus BrowserUVABackend::start()
{
NotImplemented();
#if 0 //zhoulh: can't use MTK stuff here:
#ifdef USE_MTK_NEW_BACKENDS
    uint32_t width = m_browserImpl->getGraphics().getWidth();
    uint32_t height = m_browserImpl->getGraphics().getHeight();
    InitStreamManagerAndSetResolution(width, height);
#else
    // The UVA backends can obtain the user agent from opera engine, so needn't send the UA string to it
    std::string UAString = "";
    int width = m_browserImpl->getGraphics().getWidth();
    int height = m_browserImpl->getGraphics().getHeight();
    vPiMgr_UvaInit(width, height, (char*)UAString.c_str());
#endif
#endif
    return BrowserOK;
}

BrowserStatus BrowserUVABackend::stop()
{
NotImplemented();
#if 0 // can't use MTK stuff here:
#ifdef USE_MTK_NEW_BACKENDS
    vStrmMgr_Uninit();
#else
    vPiMgr_Uninit();
#endif
#endif
    return BrowserOK;
}
