#ifndef __WEBENGINE_BROWSER_MODE_YOUTUBE_H__
#define __WEBENGINE_BROWSER_MODE_YOUTUBE_H__

#include "browser-mode.h"


#include <opera/Opera.h>

#include <opera/components/opera_controller.h>
#include <opera/components/input/input_reader.h>
#include <opera/events/LoadingFinished.h>
#include <opera/events/LoadingStarted.h>
#include <opera/events/NewWindow.h>
#include <opera/events/NewWindowBlocked.h>
#include <opera/events/PlatformMessage.h>
#include <opera/events/ScriptRequestCloseWindow.h>
#include <opera/messages/KeyInput.h>
#include "task_manager.h"


#include <string>

#include "omi_components/async_input_dispatcher_impl.h"


class InputInterceptorYoutube;

class BrowserModeYouTube : public BrowserMode
{
public:
    BrowserModeYouTube(BrowserImpl* browserImpl);

    virtual ~BrowserModeYouTube();

    virtual BrowserStatus start();
    virtual void stop();
    virtual void enter(MultiModeArgument *argument = NULL);
    virtual void leave();
    virtual void pause();
    virtual void resume();
    virtual void setUrl(std::string url);

    virtual void processOOM() { }
private:
    void setupWindows();
    void OnLoadingStarted(opera::evt::LoadingStarted loading_started);
    void OnLoadingFinished(opera::evt::LoadingFinished loading_finished);
    void OnNewWindow(opera::evt::NewWindow msg);
    void OnNewWindowBlocked(opera::evt::NewWindowBlocked msg);
    void OnPlatformMessage(opera::evt::PlatformMessage event);
    void OnScriptRequestCloseWindow(opera::evt::ScriptRequestCloseWindow event);
private:
    opera::Handle m_main_window;
//    opera::Handle m_overlay_window;
    bool m_window_created;
    InputInterceptorYoutube * m_input_interceptor;
};


#endif
