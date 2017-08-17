#include "browser-mode-off.h"
#include "browser-opera.h"
#include "browser_debug.h"
#include "browser-input.h"
#include "graphics.h"

BrowserModeOFF::BrowserModeOFF( BrowserImpl* browserImpl )
    : BrowserMode( browserImpl )
{
    BROWSER_LOG(INFO, "");
}

BrowserModeOFF::~BrowserModeOFF()
{
    BROWSER_LOG(INFO, "");
}

BrowserStatus BrowserModeOFF::start()
{
    BROWSER_LOG(INFO, "");
    return BrowserOK;
}

void BrowserModeOFF::stop()
{
    BROWSER_LOG(INFO, "");
}

void BrowserModeOFF::enter( MultiModeArgument *argument )
{
    BROWSER_LOG(INFO, "");
    BrowserInput& browserInput = browser().getBrowserInput();
    //Disable physical mouse input and key input
    browserInput.enableKeyInput( false );
    browserInput.enableMouseInput( false );

    // Graphics& graphics = browser().getGraphics();
    // graphics.requestFocus( false );

    HisenseOpera& opera = browser().getHisenseOpera();
    opera.pause();
    opera.removeAllEventListeners();
}

void BrowserModeOFF::leave()
{
    // Graphics& graphics = browser().getGraphics();
    // graphics.requestFocus( false );
    BROWSER_LOG(INFO, "");
}

void BrowserModeOFF::setUrl( std::string url )
{
    BROWSER_LOG(INFO, "");
}
