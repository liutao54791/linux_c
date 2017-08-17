#ifndef BROWSER_MODE_STORE_H
#define BROWSER_MODE_STORE_H

#include <memory>
#include <map>

#include <opera/events/LoadingFinished.h>
#include <opera/events/LoadingStarted.h>
#include <opera/events/CertificateError.h>
#include <opera/events/ScriptRequestCloseWindow.h>
#include <opera/events/PlatformMessage.h>

#include <opera/messages/KeyInput.h>

#include "browser-mode.h"
#include "language.h"
#include "omi_components/async_input_dispatcher_impl.h"

class BrowserModeStore : public BrowserMode
{
public:
    BrowserModeStore(BrowserImpl* browserImpl);

    virtual ~BrowserModeStore();

    virtual BrowserStatus start();
    virtual void stop();
    virtual void enter(MultiModeArgument *argument = NULL);
    virtual void leave();

    virtual void setUrl(std::string url);

    virtual void processOOM();
    virtual void sendMessage(const std::string & message);

    class OPERA_EXPORT ExampleInputInterceptor
    : public opera::components::AsyncInputDispatcherImpl::InputEventsInterceptor
    {
        ExampleInputInterceptor(ExampleInputInterceptor&);
        ExampleInputInterceptor& operator=(ExampleInputInterceptor&);
    public:
        ~ExampleInputInterceptor();

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

        void AddKeyEventHandler(opera::msg::KeyInput::Key key, bool(*handler)(const opera::msg::KeyInput&));

    private:

        BrowserImpl &browser() const
        {
            return * m_browserImpl;
        }
        BrowserImpl* m_browserImpl;

        typedef std::multimap<opera::msg::KeyInput::Key, bool(*)(const opera::msg::KeyInput&) > KeyEventProcessors;
        KeyEventProcessors m_key_event_processors;

        // When all toolchains will support C++11, this should be
        // changed to unique_ptr.
        std::auto_ptr<ActiveWindowGetter> active_window_getter_;
    };
private:
    opera::Handle m_status_window;
    opera::Handle m_main_window;
    opera::Handle m_status_html_view;
    AppType m_runType;

    ExampleInputInterceptor* m_input_interceptor;

    std::string m_oom_url;
    std::string m_current_url;
    bool m_OOMEntered;
    bool m_status_finished;
    void onPlatformMessage(opera::evt::PlatformMessage event);
    void loadingStarted(opera::evt::LoadingStarted event);
    void loadingFinished(opera::evt::LoadingFinished event);
    void certificateError(opera::evt::CertificateError event);
    void ScriptRequestCloseWindow(opera::evt::ScriptRequestCloseWindow event);
    bool checkNetwork(std::string url);
    std::string getHostName(std::string url);
    void waitStatusWindowFinished();
    bool filterUrl(std::string url);
};

#endif
