#include "browser-mode-opera-store.h"
#include "browser-opera.h"
#include "browser-input.h"
#include "browser_debug.h"
#include "virtual-keyboard.h"
#include "configuration.h"
#include "language.h"
#include "graphics.h"
#include "async_browser_task.h"
#include "task_manager.h"

namespace {
bool has_alt_modifier(const opera::msg::KeyInput& key_input) {
  return key_input.key_modifiers & opera::msg::KeyInput::MODIFIER_ALT;
}
}

namespace {
    class OperaStoreActiveWindowGetter : public BrowserModeOperaStore::ExampleInputInterceptor::ActiveWindowGetter {
    public:
        OperaStoreActiveWindowGetter( opera::Handle window_id )
            : m_window( window_id )
        {}
        virtual bool getActiveWindow( opera::Handle* window_id ) OPERA_OVERRIDE {
            *window_id = m_window;
            return true;
        }
    private:
        opera::Handle m_window;
    };
}


BrowserModeOperaStore::ExampleInputInterceptor::ExampleInputInterceptor(BrowserImpl* browserImpl)
    : m_browserImpl(browserImpl), active_window_getter_(NULL) {
}

bool BrowserModeOperaStore::ExampleInputInterceptor::InterceptKeyEvent(
    const opera::msg::KeyInput& key_input) {
    opera::Handle window_id;
    bool skip(false);
    if (active_window_getter_.get())
        active_window_getter_->getActiveWindow(&window_id);

        if (key_input.event_type == opera::msg::KeyInput::KEYDOWN) {
            browser().getVirtualKeyboard().EnterKeyProc(key_input);
            switch (key_input.virtual_key) {
                case opera::msg::KeyInput::OMI_KEY_POWER:
                case opera::msg::KeyInput::OMI_KEY_F12:
                    ///TODO:leave Browser mode, enter OFF mode
                    browser().setUrl(BROWSER_MODE_OFF, "");
                    skip = false;
                case opera::msg::KeyInput::OMI_KEY_F11:
                    if (window_id.IsValid()) {
                        browser().getHisenseOpera().destroyWindow(window_id);
                        skip = true;
                    }
                    break;
                case opera::msg::KeyInput::OMI_KEY_LEFT:
                    if (!has_alt_modifier(key_input))
                        break;
                // else: fall-through
                case opera::msg::KeyInput::OMI_KEY_BACK:
                    {
                        if(!(browser().getVirtualKeyboard().isShow()))//when the virtual board isn't show,send backspace,or send back
                        {
                            opera::msg::KeyInput key_input_converted = key_input;
                            key_input_converted.virtual_key = opera::msg::KeyInput::OMI_KEY_BACKSPACE;
                            browser().getHisenseOpera().sendKeyInput(key_input_converted);
                            skip = true;
                        }

                        break;
                    }
                case opera::msg::KeyInput::OMI_KEY_RIGHT: {
                    if (!has_alt_modifier(key_input))
                        break;
                    opera::Handle window_id;
                    if (active_window_getter_.get() &&
                        active_window_getter_->getActiveWindow(&window_id)) {
                        browser().getHisenseOpera().moveInHistory(window_id, 1);
                        skip = true;
                    }
                    break;
                }
                case opera::msg::KeyInput::OMI_KEY_ENTER: {
                    //skip = EnterKeyProc(key_input);
                    break;
                }
                default:
                    break;
        }
    } else {
        switch (key_input.virtual_key) {
            case opera::msg::KeyInput::OMI_KEY_BACK:
                {
                    if(!(browser().getVirtualKeyboard().isShow()))//when the virtual board isn't show,send backspace,or send back
                    {
                        opera::msg::KeyInput key_input_converted = key_input;
                        key_input_converted.virtual_key = opera::msg::KeyInput::OMI_KEY_BACKSPACE;
                        browser().getHisenseOpera().sendKeyInput(key_input_converted);
                        skip = true;
                    }

                    break;
                }
            default:
                break;
        }
    }
  return skip;
}

void BrowserModeOperaStore::ExampleInputInterceptor::setActiveWindowGetter(
    ActiveWindowGetter* active_window_getter) {
  active_window_getter_.reset(active_window_getter);
}

BrowserModeOperaStore::BrowserModeOperaStore( BrowserImpl* browserImpl )
    : BrowserMode( browserImpl )
    , m_windowCreated( false )
    , m_input_interceptor( NULL )
    , m_loadingBlocked(false)
{
    BROWSER_LOG( INFO, "" );
}

BrowserModeOperaStore::~BrowserModeOperaStore()
{
    BROWSER_LOG( INFO, "" );
}

BrowserStatus BrowserModeOperaStore::start()
{
    BROWSER_LOG(INFO, "");
    m_input_interceptor = new ExampleInputInterceptor( &browser() );
    if ( !m_input_interceptor ) {
        BROWSER_LOG(ERROR, "Failed to create input interceptor");
        return BrowserError;
    }

    return BrowserOK;
}

void BrowserModeOperaStore::stop()
{
    BROWSER_LOG(INFO, "");
    if( m_input_interceptor ) {
        delete m_input_interceptor;
        m_input_interceptor = NULL;
    }

    if (m_windowCreated) {
        HisenseOpera& opera = browser().getHisenseOpera();
        opera.destroyWindow( m_main_window );
    }
}

void BrowserModeOperaStore::enter( MultiModeArgument *argument )
{
    BROWSER_LOG( INFO, "" );

    Graphics& graphics = browser().getGraphics();
    graphics.requestFocus( true , false);

    BrowserInput& browserInput = browser().getBrowserInput();
    //Enable physical mouse input and key input
    browserInput.enableKeyInput( true );
    browserInput.enableMouseInput( false );

    //Set input interceptor
    if ( m_input_interceptor )
        browserInput.setEventsIntercept(m_input_interceptor);

    HisenseOpera& opera = browser().getHisenseOpera();
    opera.AddEventHandler( &BrowserModeOperaStore::loadingStarted, this );
    opera.AddEventHandler( &BrowserModeOperaStore::loadingFinished, this );
    opera.AddEventHandler( &BrowserModeOperaStore::loadingBlocked, this );
    opera.AddEventHandler( &BrowserModeOperaStore::PlatformMessage , this );
    opera.AddEventHandler( &BrowserModeOperaStore::ScriptRequestCloseWindow, this );
    opera.AddEventHandler( &BrowserModeOperaStore::TVStoreWindowClosed, this );
    opera.AddEventHandler( &BrowserModeOperaStore::WindowDestroyed, this );
    opera.AddStationaryEventHandler( &BrowserModeOperaStore::BrowserMemoryEvent , this );

    // enter virtual keyboard
    browser().getVirtualKeyboard().enter( m_main_window );

    std::string lang;
    opera::Handle allWindows;
    if(!browser().getLanguage().BrowserLanguageGet(lang))
    {
        opera.setPref( allWindows, "http_accept_language", lang.c_str() );
        opera.setPref( m_main_window, "intl.app_locale", lang.c_str() );
    }
    else
    {
        opera.setPref( allWindows,"http_accept_language","en-US");
        opera.setPref( m_main_window, "intl.app_locale", "en-US" );
    }

    opera.resume();
}

void BrowserModeOperaStore::leave()
{
    BROWSER_LOG(INFO, "");

    BrowserInput& browserInput = browser().getBrowserInput();
    //Disable physical mouse input and key input
    browserInput.enableKeyInput( false );
    browserInput.enableMouseInput( false );

    HisenseOpera& opera = browser().getHisenseOpera();

    // Will remove all event listener from store
    opera.pause();
    opera.removeAllEventListeners();

    browser().getVirtualKeyboard().leave();

    opera.destroyWindow( m_status_window );
    opera.destroyWindow( m_main_window );
    opera.destroyWindow( m_dialog_window );
    m_windowCreated = false;

    browserInput.setEventsIntercept(NULL);
    Graphics& graphics = browser().getGraphics();
    graphics.lostFocus();
    BROWSER_LOG(INFO, "");
}

void BrowserModeOperaStore::setUrl( std::string url )
{
    BROWSER_LOG( INFO, "url = %s", url.c_str() );
    reInit();
    HisenseOpera& opera = browser().getHisenseOpera();

    ///TODO:The Opera TV Store window doesn't support LoadURL, whether to create Store window again?
    if ( !m_windowCreated ) {
        m_status_window = opera.createStatusWindow();
        BROWSER_LOG(INFO, "Create new window");
        m_main_window = opera.createStoreWindow( OPERA_TV_STORE_WINDOW, url );
        browser().getVirtualKeyboard().setTargetWindow(m_main_window);
        // notify js of videocontrol not work
        opera.sendPlatformEvent( m_main_window, "stop-videocontrol" );
        opera.setPref( m_main_window, "content_resolution", HD );
        opera.setPref( m_main_window, "default_background_color", "#000000" );
        opera.setPref( m_main_window, "allow_scripts_to_close_windows", "true");
        opera.setPref( m_main_window, "notify_scripts_closing_windows", "true");
        opera.setPref( m_main_window, "max_disk_cache_size","20480");

        //m_status_window = opera.createStatusWindow();
        //opera.setPref( m_status_window, "transparent", "true" );
        // Move the status window to the top, hide status window, and focus main window
        opera.setStackAbove( m_status_window, m_main_window);
        //opera.hide( m_status_window );
        opera.focus( m_main_window );

        //send window id to input interceptor
        if ( m_input_interceptor )
            m_input_interceptor->setActiveWindowGetter( new OperaStoreActiveWindowGetter( m_main_window ) );
        BROWSER_LOG(INFO, "Create diaglog window");
        m_dialog_window = opera.createWindow();
        opera.setStackAbove(m_dialog_window,m_status_window);
        m_windowCreated = true;
    }
}

void BrowserModeOperaStore::processOOM()
{
    showOOM();
}

void BrowserModeOperaStore::loadingStarted( opera::evt::LoadingStarted event )
{
    BROWSER_LOG(ALWAYS, "Loading started window: %s %s", event.window_id.str().c_str(), event.url.c_str());
    if ( event.window_id == m_main_window ) {
        BROWSER_LOG(ALWAYS, "m_main_window loading start.");
        HisenseOpera& opera = browser().getHisenseOpera();
        // Show status window, focus main window
        opera.show( m_status_window );
        opera.focus( m_main_window );
    }
}

void BrowserModeOperaStore::loadingFinished( opera::evt::LoadingFinished event )
{
    HisenseOpera& opera = browser().getHisenseOpera();
    std::string status_str;
    if ( event.window_id == m_main_window && !m_loadingBlocked ) {
        opera.hide( m_status_window );
        opera.show(m_main_window);
        opera.focus( m_main_window );
        switch (event.status) {
            case opera::evt::LoadingFinished::LOAD_SUCCEEDED:
                status_str = "succeeded";
                break;
            case opera::evt::LoadingFinished::LOAD_FAILED:
                status_str = "failed";
                break;
            case opera::evt::LoadingFinished::LOAD_ABORTED:
                status_str = "aborted";
                break;
            default:
                status_str = "other";
        }
    }else{
        BROWSER_LOG(INFO,"do nothing..................");
    }

    BROWSER_LOG(ALWAYS, "Loading finished (%s) window: %s %s", status_str.c_str(), event.window_id.str().c_str(), event.url.c_str());
}

void BrowserModeOperaStore::loadingBlocked( opera::evt::LoadingBlocked event)
{
    m_loadingBlocked = true;
    HisenseOpera& opera = browser().getHisenseOpera();
    std::string dialog_url = browser().getConfiguration().getValueByKey("DialogUrl");
    opera.hide(m_status_window);
    opera.loadURL(m_dialog_window, dialog_url);
    opera.show(m_dialog_window);
    opera.focus(m_dialog_window);
}
void BrowserModeOperaStore::PlatformMessage( opera::evt::PlatformMessage event)
{
    if(event.message == "exitCurrentMode") {

        browser().getTaskManager().postTask( hisense::AsyncBrowserTask::create(&browser(),"pause"));
        return;
    }
    HisenseOpera& opera = browser().getHisenseOpera();
    if(event.window_id == m_dialog_window)
    {
        if( "hide_dialog_window"== event.message )
        {
            opera.loadURL(m_dialog_window , "about:blank");
            opera.hide(m_dialog_window);
            opera.focus(m_main_window);
            m_loadingBlocked = false;
        }
        else if( "exit_store" == event.message )
        {
            browser().getTaskManager().postTask( hisense::AsyncBrowserTask::create(&browser(),"pause"));
        }
    }
}

void BrowserModeOperaStore::TVStoreWindowClosed(opera::evt::TVStoreWindowClosed event)
{
    BROWSER_LOG(INFO, "Got TVStoreWindowClosed window_id %s", event.window_id.str().c_str());
    browser().getTaskManager().postTask( hisense::AsyncBrowserTask::create(&browser(),"pause"));
}

void BrowserModeOperaStore::ScriptRequestCloseWindow(opera::evt::ScriptRequestCloseWindow event)
{
    BROWSER_LOG(INFO, "Got ScriptRequestCloseWindow window_id %s opened_by_dom %d url %s",
        event.id.str().c_str(), event.opened_by_dom, event.url.c_str());
    browser().getTaskManager().postTask( hisense::AsyncBrowserTask::create(&browser(),"pause"));
}

void BrowserModeOperaStore::WindowDestroyed(opera::evt::WindowDestroyed event)
{
    BROWSER_LOG(INFO, "Got WindowDestroyed window_id %s", event.window_id.str().c_str());
    HisenseOpera& opera = browser().getHisenseOpera();
    opera.hide(m_status_window);
    opera.focus(m_main_window);
}

void BrowserModeOperaStore::BrowserMemoryEvent( opera::evt::BrowserMemoryEvent event)
{
    if((event.state == opera::evt::BrowserMemoryEvent::OOM) ||
       (event.state == opera::evt::BrowserMemoryEvent::LOW))
    {
        showOOM();
    }
}

void BrowserModeOperaStore::showOOM()
{
    HisenseOpera& opera = browser().getHisenseOpera();
    m_loadingBlocked = true;
    opera.hide(m_status_window);
    opera.loadURL(m_status_window, "about:blank");
    opera.stopLoading(m_main_window);
    browser().getVirtualKeyboard().leave();
    std::string dialog_url = browser().getConfiguration().getValueByKey("DialogUrl");
    opera.loadURL(m_dialog_window, dialog_url + "?reason=oom");
    opera.show(m_dialog_window);
    opera.focus(m_dialog_window);
}

void BrowserModeOperaStore::reInit(void)
{
    if(m_windowCreated)
    {
        HisenseOpera& opera = browser().getHisenseOpera();
        BROWSER_LOG(ALWAYS,"restart opera app.....................");
        opera.destroyWindow( m_status_window );
        opera.destroyWindow( m_main_window );
        opera.destroyWindow( m_dialog_window );
        m_windowCreated = false;
    }
}

