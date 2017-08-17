#ifndef BROWSER_MODE_OIPF_H
#define BROWSER_MODE_OIPF_H

#include <vector>

#include <opera/events/LoadingStarted.h>
#include <opera/events/LoadingFinished.h>
#include <opera/events/OIPFApplicationActivated.h>
#include <opera/events/OIPFApplicationDeactivated.h>
#include <opera/events/OIPFApplicationStarted.h>
#include <opera/events/OIPFApplicationStopped.h>
#include <opera/events/OIPFApplicationStartFailed.h>
#include <opera/events/HbbTVApplicationStarted.h>
#include <opera/events/HbbTVApplicationStopped.h>
#include <opera/events/ScriptRequestCloseWindow.h>

#include "omi_components/async_input_dispatcher_impl.h"

#include "browser-mode.h"
#include "hbbtvomi_components/hbbtv_controller.h"
#include "hbbtvomi_components/hbbtv/hbbtv_integration.h"

typedef std::vector<std::shared_ptr<opera::evt::OIPFApplicationStarted>> RunningApps;

class UrlFilter;

class BrowserModeOIPF : public BrowserMode
{
public:
    BrowserModeOIPF(BrowserImpl* browserImpl);

    virtual ~BrowserModeOIPF();

    virtual BrowserStatus start();
    virtual void stop();
    virtual void enter(MultiModeArgument *argument = NULL);
    virtual void leave();

    virtual void setUrl(std::string url);

    virtual void processOOM() { }

private:

    class InputInterceptor
    : public opera::components::AsyncInputDispatcherImpl::InputEventsInterceptor
    {
    public:
        InputInterceptor(BrowserImpl* browserImpl);

        virtual bool WouldConsume(const opera::msg::KeyInput& key_input) OPERA_OVERRIDE;
        virtual bool InterceptKeyEvent(const opera::msg::KeyInput& key_input) OPERA_OVERRIDE;

    private:
        InputInterceptor(InputInterceptor&);
        InputInterceptor& operator=(InputInterceptor&);

        BrowserImpl & browser() const
        {
            return * m_browserImpl;
        }
        BrowserImpl* m_browserImpl;

    };

    opera::Handle CreateHbbTVWindow(
                                    std::vector<opera::msg::CreateHbbTVWindow::PlatformProfile> platform_profiles);

    opera::Handle hbbtv_window_;

    //opera::Handle m_main_window;
    InputInterceptor* m_input_interceptor;


    void OnHbbTVKeysetChanged(opera::evt::HbbTVKeysetChanged evt);
    void loadingStarted(opera::evt::LoadingStarted event);
    void loadingFinished(opera::evt::LoadingFinished event);
    void OnHbbTVApplicationStopped(opera::evt::HbbTVApplicationStopped event);
    void ScriptRequestCloseWindow(opera::evt::ScriptRequestCloseWindow event);


};

#endif

