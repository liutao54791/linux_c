
#include "browser-mode-youtube.h"
#include "browser-opera.h"
#include "browser_debug.h"
#include "browser-input.h"
#include "graphics.h"
#include "virtual-keyboard.h"
#include "configuration.h"
#include <omi_components/input/oem_keycodes.h>
#include "NotImplemented.h"

using opera::msg::KeyInput;

class InputInterceptorYoutube
: public opera::components::AsyncInputDispatcherImpl::InputEventsInterceptor
{
public:

    InputInterceptorYoutube(BrowserImpl* browser)
    : m_browserImpl(browser)
    {
    }

    virtual bool InterceptKeyEvent(const opera::msg::KeyInput& key_input)
    {
        //Convert some keys:
        const static struct KeyMap
        {
            int key_code;
            int new_key_code;
            const char* new_key_value;

        } convert_map[] = {

            {KeyInput::OMI_KEY_LEFT, 0x25, ""}, //Left (keyCode = 0 x 2 5 )
            {KeyInput::OMI_KEY_RIGHT, 0x27, ""}, //Right (keyCode = 0 x 2 7 )
            {KeyInput::OMI_KEY_UP, 0x26, ""}, //Up (keyCode = 0 x 2 6 )
            {KeyInput::OMI_KEY_DOWN, 0x28, ""}, //Down (keyCode = 0 x 2 8 )
            {KeyInput::OMI_KEY_ENTER, 0x0D, ""}, //Enter (keyCode = 0 x 0 D )
            {KeyInput::OMI_KEY_BACK, 0x1B, ""}, //Escape / Back / Return (keyCode = 0 x 1 B )

            {KeyInput::OMI_KEY_PLAY, 0xFA, ""}, //Play (keyCode = 0 x F A )
            {KeyInput::OMI_KEY_PAUSE, 0x13, ""}, //Pause (keyCode = 0 x 1 3 )
            {KeyInput::OMI_KEY_PLAY_PAUSE, 0xB3, ""},//Play/Pause (keyCode = 0 x B 3 )

            {KeyInput::OMI_KEY_STOP, 0xB2, ""}, //Stop (keyCode = 0 x B 2 )
            {KeyInput::OMI_KEY_FAST_FWD, 0xE4, ""}, //Fast Forward (keyCode = 0 x E 4 )
            {KeyInput::OMI_KEY_REWIND, 0xE3, ""}, //Rewind (keyCode = 0 x E 3 )
            {KeyInput::OMI_KEY_SPACE, 0x20, ""}, // Space (keyCode = 0 x 2 0 )
            {KeyInput::OMI_KEY_BACKSPACE, 0x08, ""},  //Backspace (keyCode = 0 x 0 8 )
            {KeyInput::OMI_KEY_DELETE, 0x47, ""}, //Delete (keyCode = 0 x 4 7 )
            {KeyInput::OMI_KEY_BROWSER_SEARCH,0x53, ""}, //Search (keyCode = 0 x 5 3 )
                                                  //Previous (keyCode = 0 x B 1 )
                                                   //Next (keyCode = 0 x B 0 )
            {KeyInput::OMI_KEY_SUBTITLE, 0xAF, ""}, //Closed Captions (keyCode = 0 x A F )
            {KeyInput::OMI_KEY_RED, 0x193, ""},//Red (keyCode = 0x193)
            {KeyInput::OMI_KEY_GREEN, 0x194, ""},// Green (keyCode = 0x194)
            {KeyInput::OMI_KEY_YELLOW, 0x195, ""},//Yellow (keyCode = 0x195)
            {KeyInput::OMI_KEY_BLUE, 0x196, ""},//Blue (keyCode = 0x196)
            {OEM_KEY(YOUTUBE), 0xAC, ""},//YouTube button (keyCode = 0 x A C )

            {0, 0, ""}
        };
        for (int i = 0; i<sizeof (convert_map) / sizeof (convert_map[0]); i++) {
            if (key_input.virtual_key == convert_map[i].key_code) {
                opera::msg::KeyInput key_input_converted = key_input;
                key_input_converted.virtual_key = (opera::msg::KeyInput::Key) convert_map[i].new_key_code;
                key_input_converted.key_value = convert_map[i].new_key_value;
                browser().getHisenseOpera().sendKeyInput(key_input_converted);
                return true;
            }
        }

        //Blacklist some keys If needed:
        const KeyInput::Key blacklist_keys[] = {
           KeyInput:: OMI_KEY_UNDEFINED

          };

        for(int i=0;i< sizeof(blacklist_keys)/sizeof(blacklist_keys[0]);i++)
        {
            if(blacklist_keys[i] ==key_input.virtual_key ){
                return true;
            }
        }

        //Send All Other keys:
        return false;
    }

private:
    InputInterceptorYoutube(InputInterceptorYoutube&); //No Body
    InputInterceptorYoutube& operator=(InputInterceptorYoutube&); //No Body

    BrowserImpl & browser() const
    {
        return * m_browserImpl;
    }
    BrowserImpl* m_browserImpl;
};

BrowserModeYouTube::BrowserModeYouTube(BrowserImpl* browserImpl)
: BrowserMode(browserImpl)
, m_window_created(false)
{
    BROWSER_LOG(INFO, "");
}

BrowserModeYouTube::~BrowserModeYouTube()
{
    BROWSER_LOG(INFO, "");
}

BrowserStatus BrowserModeYouTube::start()
{
    BROWSER_LOG(INFO, "");
    m_input_interceptor = new InputInterceptorYoutube(&browser());
    return BrowserOK;
}

void BrowserModeYouTube::pause()
{
    BROWSER_LOG(INFO, "");

}

void BrowserModeYouTube::resume()
{
    BROWSER_LOG(INFO, "");

}

void BrowserModeYouTube::stop()
{
    BROWSER_LOG(INFO, "");
    if (m_input_interceptor) {
        delete m_input_interceptor;
        m_input_interceptor = NULL;
    }
}

void BrowserModeYouTube::setupWindows()
{
    HisenseOpera& opera = browser().getHisenseOpera();
    if (!m_window_created) {
        BROWSER_LOG(INFO, "Create new window for YouTube");
        m_main_window = opera.createWindow();
        opera.setPref(m_main_window, "content_resolution", FHD);
        opera.setPref(m_main_window, "default_background_color", "#000000");
        opera.setPref(m_main_window, "allow_scripts_to_close_windows", "true");
        opera.setPref(m_main_window, "notify_scripts_closing_windows", "true");
        opera.setPref(m_main_window, "max_disk_cache_size", "20480");

        //Enable TTS based on system settings
        bool tts_enabled = browser().getConfiguration().getTTSEnable();

        if(tts_enabled)
        {
            BLOGI("Enable TTS for Youtube");
            opera.setPref(m_main_window, "chromevox_enabled.override", "true");
        }
        else
        {
            BLOGI("Disable  TTS for Youtube");
            opera.setPref(m_main_window, "chromevox_enabled.override", "false");
        }
        std::string highlight_enabled =browser().getConfiguration().getValueByKey("TTSHighlightEnabled");
        //Default to false:
        BLOGI("TTSHighlightEnabled: %s", highlight_enabled.c_str());
        if(highlight_enabled.empty()){
            highlight_enabled = "false";
        }
        opera.setPref(m_main_window, "accessibility_focus_highlight_enabled", highlight_enabled);

        std::string headUA = opera.getPref(m_main_window, "user_agent");
        std::string UASuffix = browser().getConfiguration().getUAForApp(YOUTUBE);
        opera.setPref(m_main_window, "user_agent", headUA + UASuffix);
        // char lang[LANG_MAX_LEN] = {0};
        std::string lang;
        if (!browser().getLanguage().BrowserLanguageGet(lang)) {
            opera.setPref(opera::Handle(-1), "http_accept_language", lang.c_str());
            opera.setPref(opera::Handle(-1), "intl.app_locale", lang.c_str());
         } else {
             opera.setPref(opera::Handle(-1), "http_accept_language", "en-US");
             opera.setPref(opera::Handle(-1), "intl.app_locale", "en-US");
         }
        ////Create overlay window:
        //m_overlay_window = opera.createWindow();
        //opera.setPref(m_overlay_window, "content_resolution", FHD);
        //opera.setPref(m_overlay_window, "transparent", "true");
        //opera.setStackAbove(m_overlay_window, m_main_window);
        //opera.hide(m_overlay_window);

        m_window_created = true;
    }
    opera.focus(m_main_window);
    //TODO:
    // opera.loadURL(m_overlay_window, overlay_url);
    // enter virtual keyboard
    // browser().getVirtualKeyboard().enter(m_main_window);
}

void BrowserModeYouTube::enter(MultiModeArgument *argument)
{
    BROWSER_LOG(INFO, "");
    //setup graphics:
    Graphics& graphics = browser().getGraphics();
    graphics.requestFocus(true, false);
    //setup input:
    BrowserInput& browserInput = browser().getBrowserInput();
    browserInput.setEventsIntercept(m_input_interceptor);

    //Enable physical mouse input and key input
    browserInput.enableKeyInput(true);
    browserInput.enableMouseInput(false);
    //TODO: setup input interceptor
    HisenseOpera& opera = browser().getHisenseOpera();
    opera.AddEventHandler(&BrowserModeYouTube::OnLoadingStarted, this);
    opera.AddEventHandler(&BrowserModeYouTube::OnLoadingFinished, this);
    opera.AddEventHandler(&BrowserModeYouTube::OnNewWindow, this);
    opera.AddEventHandler(&BrowserModeYouTube::OnNewWindowBlocked, this);
    opera.AddEventHandler(&BrowserModeYouTube::OnPlatformMessage, this);
    opera.AddEventHandler(&BrowserModeYouTube::OnScriptRequestCloseWindow, this);
    this->setupWindows();
    opera.resume();
}

void BrowserModeYouTube::leave()
{
    BROWSER_LOG(INFO, "");
    BrowserInput& browserInput = browser().getBrowserInput();
    browserInput.setEventsIntercept(NULL);
    //Disable physical mouse input and key input
    browserInput.enableKeyInput(false);
    browserInput.enableMouseInput(false);

    HisenseOpera& opera = browser().getHisenseOpera();
    // Will remove all event listener
    opera.hide(m_main_window);
    opera.pause();
    opera.removeAllEventListeners();
    //browser().getVirtualKeyboard().leave();

    if (m_window_created) {
        m_window_created = false;
        opera.destroyWindow(m_main_window);
        //opera.destroyWindow(m_overlay_window);
    }
    Graphics& graphics = browser().getGraphics();
    graphics.lostFocus();
}

void BrowserModeYouTube::setUrl(std::string url)
{
    if (!m_window_created) {
        BLOGE("Please Enter Youtube mode First.");
        return;
    }
    HisenseOpera& opera = browser().getHisenseOpera();
    opera.loadURL(m_main_window, url);
}

void BrowserModeYouTube::OnLoadingStarted(opera::evt::LoadingStarted loading_started)
{
    BLOGI("win_id=%d,url=%s", loading_started.window_id, loading_started.url.c_str());
    NotImplemented();
    //TODO: show loading indicator
}

void BrowserModeYouTube::OnLoadingFinished(opera::evt::LoadingFinished loading_finished)
{
    BLOGI("win_id=%d,url=%s\nstatus=%s,err=%d,msg=%s",
          loading_finished.window_id,
          loading_finished.url.c_str(),
          opera::evt::LoadingFinished::ToString(loading_finished.status).c_str(),
          loading_finished.error_code,
          loading_finished.error_message.c_str()
          );


    NotImplemented();
    //TODO: hide loading indicator
}

void BrowserModeYouTube::OnNewWindow(opera::evt::NewWindow msg)
{
    BLOGI("win_id=%d,parent=%d", msg.window_id, msg.opener);
    NotImplemented();
    //TODO: deal with multi-window issues based on requirements:
    //1. close old window show new window
    //2. close new window show url in old window
    //3. close new window show  old window
    //4. use multi-tab window manager
    // More advanced integration's should add window management and show
    // this new window or maybe just close it.
}

void BrowserModeYouTube::OnNewWindowBlocked(opera::evt::NewWindowBlocked msg)
{
    NotImplemented();
    //TODO: show dialog in overlay window
}

void BrowserModeYouTube::OnPlatformMessage(opera::evt::PlatformMessage event)
{
    BLOGI("windowid=%d,msg=%s", event.window_id, event.message.c_str());

}
void BrowserModeYouTube::OnScriptRequestCloseWindow(opera::evt::ScriptRequestCloseWindow event)
{
    BLOGI("YT OnScriptRequestCloseWindow window close windowid=%s.", event.id.str().c_str() );

    if(m_main_window == event.id)
    {
        BLOGI( "YT Got ScriptRequestCloseWindow window_id %s opened_by_dom %d url %s",
                    event.id.str().c_str(), event.opened_by_dom, event.url.c_str());
        browser().getTaskManager().postTask(hisense::AsyncBrowserTask::create(&browser(), "pause"));
    }
    else
    {
        BLOGE("YT window close windowid %s is not mainwindow!!!", event.id.str().c_str() );
    }
}
