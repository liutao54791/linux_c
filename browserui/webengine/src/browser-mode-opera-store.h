#ifndef BROWSER_MODE_OPERA_STORE_H
#define BROWSER_MODE_OPERA_STORE_H

#include <opera/events/LoadingFinished.h>
#include <opera/events/LoadingStarted.h>
#include <opera/events/LoadingBlocked.h>
#include <opera/events/PlatformMessage.h>
#include <opera/events/ScriptRequestCloseWindow.h>
#include <opera/events/TVStoreWindowClosed.h>
#include <opera/events/BrowserMemoryEvent.h>
#include <opera/events/WindowDestroyed.h>

#include <opera/messages/KeyInput.h>

#include "browser-mode.h"
#include "omi_components/async_input_dispatcher_impl.h"

class BrowserModeOperaStore : public BrowserMode
{
public:
    BrowserModeOperaStore(BrowserImpl* browserImpl);

    virtual ~BrowserModeOperaStore();

    virtual BrowserStatus start();
    virtual void stop();
    virtual void enter(MultiModeArgument *argument = NULL);
    virtual void leave();

    virtual void setUrl(std::string url);

    virtual void processOOM();

    class OPERA_EXPORT ExampleInputInterceptor
    : public opera::components::AsyncInputDispatcherImpl::InputEventsInterceptor
    {
        ExampleInputInterceptor(ExampleInputInterceptor&);
        ExampleInputInterceptor& operator=(ExampleInputInterceptor&);
    public:

        /**
         * Used for getting current active window handle from the integration
         * in order to send messages to that window. For this purpose it
         * would be better to use some callable template argument, but in
         * this code we do not have access to such things.
         * */
        class ActiveWindowGetter
        {
        public:
            /**
             * @param window_id the active window ID set by the callee if the
             * interceptor should be allowed to perform actions on a window.
             * @return true if window_id output argument was set during the call.
             * */
            virtual bool getActiveWindow(opera::Handle* window_id) = 0;

            virtual ~ActiveWindowGetter() { }
        };
        ExampleInputInterceptor(BrowserImpl* browserImpl);

        virtual bool InterceptKeyEvent(const opera::msg::KeyInput& key_input) OPERA_OVERRIDE;

        /**
         * Deletes the old one.
         * */
        void setActiveWindowGetter(ActiveWindowGetter* active_window_getter);

    private:

        BrowserImpl& browser() const
        {
            return * m_browserImpl;
        }
        BrowserImpl* m_browserImpl;

        // When all toolchains will support C++11, this should be
        // changed to unique_ptr.
        std::auto_ptr<ActiveWindowGetter> active_window_getter_;
    };

private:
    opera::Handle m_status_window;
    opera::Handle m_main_window;
    opera::Handle m_dialog_window;

    bool m_windowCreated;

    ExampleInputInterceptor* m_input_interceptor;
    bool m_loadingBlocked;

    void loadingStarted(opera::evt::LoadingStarted event);
    void loadingFinished(opera::evt::LoadingFinished event);
    void loadingBlocked(opera::evt::LoadingBlocked event);
    void PlatformMessage(opera::evt::PlatformMessage event);
    void TVStoreWindowClosed(opera::evt::TVStoreWindowClosed event);
    void ScriptRequestCloseWindow(opera::evt::ScriptRequestCloseWindow event);
    void WindowDestroyed(opera::evt::WindowDestroyed event);
    void BrowserMemoryEvent(opera::evt::BrowserMemoryEvent event);
    void reInit(void);

    void showOOM();
};

#endif

