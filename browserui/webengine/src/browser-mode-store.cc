#include "browser-mode-store.h"
#include "browser-opera.h"
#include "browser-input.h"
#include "browser_debug.h"
#include "configuration.h"
#include "virtual-keyboard.h"
#include "language.h"
#include "browser-util.h"
#include "graphics.h"
#include "async_browser_task.h"
#include "task_manager.h"
#include <unistd.h>
#include <stdlib.h>
#include <json/json.h>
//Opera no longer supports  HTML view since 4.8
#undef STATUS_VIEW

#define NETRANGE_UA "Mozilla/5.0 (Linux armv7l) AppleWebKit/537.36" \
                     "(KHTML, like Gecko) Chrome/42.0.2311.107 Safari/537.36 OPR/29.0.1803.0" \
                     " OMI/4.5.23.27.ALSAN5.0 (Hisense;LTDN55K321UWT;F0.00123) CE-HTML/1.0 NETRANGEMMH HbbTV/1.2.1 MTK5657SA"


namespace {

bool has_alt_modifier(const opera::msg::KeyInput& key_input)
{
    return key_input.key_modifiers & opera::msg::KeyInput::MODIFIER_ALT;
}
}

namespace {

class StoreActiveWindowGetter : public BrowserModeStore::ExampleInputInterceptor::ActiveWindowGetter
{
public:

    StoreActiveWindowGetter(opera::Handle window_id)
    : m_window(window_id)
    {
    }

    virtual bool getActiveWindow(opera::Handle* window_id) OPERA_OVERRIDE
    {
        *window_id = m_window;
        return true;
    }
private:
    opera::Handle m_window;
};
}

BrowserModeStore::ExampleInputInterceptor::ExampleInputInterceptor(BrowserImpl* browserImpl)
: m_browserImpl(browserImpl), active_window_getter_(NULL)
{
}

BrowserModeStore::ExampleInputInterceptor::~ExampleInputInterceptor()
{
    m_key_event_processors.clear();
}

void BrowserModeStore::ExampleInputInterceptor::AddKeyEventHandler
(opera::msg::KeyInput::Key key, bool(*handler)(const opera::msg::KeyInput&))
{
    m_key_event_processors.insert(std::make_pair(key, handler));
}

bool BrowserModeStore::ExampleInputInterceptor::InterceptKeyEvent(
        const opera::msg::KeyInput& key_input)
{
    opera::Handle window_id;
    if (active_window_getter_.get())
        active_window_getter_->getActiveWindow(&window_id);

    if (key_input.event_type == opera::msg::KeyInput::KEYDOWN) {
        browser().getVirtualKeyboard().EnterKeyProc(key_input);

        switch (key_input.virtual_key)
        {
            case opera::msg::KeyInput::OMI_KEY_POWER:
                ///TODO:leave Browser mode, enter OFF mode
                browser().setUrl(BROWSER_MODE_OFF, "");
                return false;
            case opera::msg::KeyInput::OMI_KEY_BACK:
            {
                opera::msg::KeyInput key_input_converted = key_input;
                key_input_converted.virtual_key = opera::msg::KeyInput::OMI_KEY_BACKSPACE;
                browser().getHisenseOpera().sendKeyInput(key_input_converted);
                return true;
                break;
            }
            default:
                break;
        }
    } else {
        switch (key_input.virtual_key)
        {
            case opera::msg::KeyInput::OMI_KEY_BACK:
            {
                opera::msg::KeyInput key_input_converted = key_input;
                key_input_converted.virtual_key = opera::msg::KeyInput::OMI_KEY_BACKSPACE;
                browser().getHisenseOpera().sendKeyInput(key_input_converted);
                return true;
                break;
            }
            default:
                break;
        }
    }

    std::pair<KeyEventProcessors::const_iterator, KeyEventProcessors::const_iterator> suitable_processors
            = m_key_event_processors.equal_range(key_input.virtual_key);
    for (KeyEventProcessors::const_iterator it = suitable_processors.first; it != suitable_processors.second; ++it) {
        if ((it->second == NULL) || (it->second(key_input))) {
            return true;
        }
    }

    return false;
}

void BrowserModeStore::ExampleInputInterceptor::setActiveWindowGetter(
        ActiveWindowGetter* active_window_getter)
{
    active_window_getter_.reset(active_window_getter);
}

BrowserModeStore::BrowserModeStore(BrowserImpl* browserImpl)
: BrowserMode(browserImpl)
, m_runType(NETRANGE)
, m_input_interceptor(NULL)
, m_OOMEntered(false)
{
    BROWSER_LOG(INFO, "");
}

BrowserModeStore::~BrowserModeStore()
{
    BROWSER_LOG(INFO, "");
}

BrowserStatus BrowserModeStore::start()
{
    BROWSER_LOG(INFO, "");
    m_input_interceptor = new ExampleInputInterceptor(&browser());
    if (!m_input_interceptor) {
        BROWSER_LOG(ERROR, "Failed to create input interceptor");
        return BrowserError;
    }

    //   m_input_interceptor->AddKeyEventHandler(opera::msg::KeyInput::OMI_KEY_ENTER, EnterKeyProc);

    m_oom_url = browser().getConfiguration().getValueByKey("OOMUrl");

    return BrowserOK;
}

void BrowserModeStore::stop()
{
    BROWSER_LOG(INFO, "");
    if (m_input_interceptor) {
        delete m_input_interceptor;
        m_input_interceptor = NULL;
    }

    HisenseOpera& opera = browser().getHisenseOpera();
    if (m_main_window.IsValid())
        opera.destroyWindow(m_main_window);
}

void BrowserModeStore::enter(MultiModeArgument *argument)
{
    BROWSER_LOG(INFO, "");
    m_OOMEntered = false;

    Graphics& graphics = browser().getGraphics();
    graphics.requestFocus(true, false);

    BrowserInput& browserInput = browser().getBrowserInput();
    //Disable physical mouse input and enable key input
    browserInput.enableKeyInput(true);
    browserInput.enableMouseInput(false);

    HisenseOpera& opera = browser().getHisenseOpera();

    BrowserLanguage& language = browser().getLanguage();
    m_runType = argument->type;

    BROWSER_LOG(ALWAYS, "The type is : %d", m_runType);
    m_main_window = opera.createStoreWindow(OTHER_STORE_WINDOW, "");
    opera.setPref( m_main_window, "default_background_color", "#000000" );



    opera.hide(m_main_window);
    //std::string headUA = browser().getConfiguration().getValueByKey("NetrangeUA");
    std::string headUA = opera.getPref(m_main_window, "user_agent");
    std::string UASuffix = browser().getConfiguration().getUAForApp(m_runType);
    BROWSER_LOG(INFO, "headUA:%s", headUA.c_str());

    // 1080P for GameCenter and Hisense Application
    switch (m_runType)
    {
        case ACCEDO:
            opera.setPref(m_main_window, "user_agent", headUA + " MST6586");
            opera.setPref(m_main_window, "content_resolution", FHD);
            break;
        case HISENSE:
            opera.setPref(m_main_window, "content_resolution", FHD);
            opera.setPref(m_main_window, "transparent", "true");
            opera.setPref(m_main_window, "default_cert_error_page_enabled", "false");
            //don't use the userAgent of mobile for dropbox
            //opera.setPref( m_main_window, "user_agent", "Mozilla/5.0 (Android; Mobile; rv:14.0) Gecko/14.0 Firefox/14.0");
            break;
        case NETRANGE:
            opera.setPref(m_main_window, "user_agent", headUA + UASuffix);
            opera.setPref(m_main_window, "content_resolution", HD);
            opera.setPref(m_main_window, "transparent", "true");
            break;
        case FOXXUM:
            opera.setPref(m_main_window, "user_agent", headUA + UASuffix);
            opera.setPref(m_main_window, "content_resolution", HD);
            break;
        default:
            opera.setPref(m_main_window, "user_agent", headUA + UASuffix);
            opera.setPref(m_main_window, "content_resolution", HD);
            break;
    }

    // notify js of videocontrol not work
    //opera.sendPlatformEvent( m_main_window, "stop-videocontrol" );

    // enter virtual keyboard
    browser().getVirtualKeyboard().enter(m_main_window);

#if 0
    m_status_html_view = opera.createStatusHTMLView(m_main_window);
#else
    m_status_window = opera.createStatusWindow(m_main_window);
    opera.disableScreenReaderHighlight(m_status_window);
    opera.setPref(m_status_window, "transparent", "true");
#endif

    opera.setPref(m_main_window, "transparent", "true");
    opera.setPref(m_main_window, "allow_scripts_to_close_windows", "true");
    opera.setPref(m_main_window, "notify_scripts_closing_windows", "true");
    opera.setPref(m_main_window, "block_error_pages", "true");
    opera.setPref(m_main_window, "max_disk_cache_size", "20480");
    opera.setPref(m_main_window, "block_mixed_xhr_content", "false");
    opera.setPref(m_main_window, "spatial_navigation_enabled", "true");

    //char lang[LANG_MAX_LEN]= {0};
    std::string lang;
    if (!browser().getLanguage().BrowserLanguageGet(lang)) {
        opera.setPref(opera::Handle(-1), "http_accept_language", lang.c_str());
        opera.setPref(opera::Handle(-1), "intl.app_locale", lang.c_str());
    } else {
        opera.setPref(opera::Handle(-1), "http_accept_language", "en-US");
        opera.setPref(opera::Handle(-1), "intl.app_locale", "en-US");
    }
    opera.setPref( m_main_window, "font_family.cursive", "Frutiger LT Arabic 55 Roman" );
    opera.setPref( m_main_window, "font_family.default", "Frutiger LT Arabic 55 Roman" );
    opera.setPref( m_main_window, "font_family.fantasy", "Frutiger LT Arabic 55 Roman" );
    opera.setPref( m_main_window, "font_family.fixed", "Frutiger LT Arabic 55 Roman" );
    opera.setPref( m_main_window, "font_family.sans", "Frutiger LT Arabic 55 Roman" );
    opera.setPref( m_main_window, "font_family.serif", "Frutiger LT Arabic 55 Roman" );

    //send window id to input interceptor
    if (m_input_interceptor)
        m_input_interceptor->setActiveWindowGetter(new StoreActiveWindowGetter(m_main_window));

#if 0
    //opera.hide( m_status_html_view );
    opera.show(m_status_html_view);
#else
    // Move the status window to the top, hide status window, and focus main window
    opera.setStackAbove(m_status_window, m_main_window);
    opera.hide(m_status_window);
    //opera.show( m_status_window );
#endif



    //Set input interceptor
    if (m_input_interceptor)
        browserInput.setEventsIntercept(m_input_interceptor);

    opera.AddEventHandler(&BrowserModeStore::loadingStarted, this);
    opera.AddEventHandler(&BrowserModeStore::loadingFinished, this);
    opera.AddEventHandler(&BrowserModeStore::certificateError, this);
    opera.AddEventHandler(&BrowserModeStore::ScriptRequestCloseWindow, this);
    opera.AddEventHandler(&BrowserModeStore::onPlatformMessage, this);
    opera.resume();
}

void BrowserModeStore::leave()
{
    BROWSER_LOG(INFO, "");

    BrowserInput& browserInput = browser().getBrowserInput();
    //Disable physical mouse input and key input
    browserInput.enableKeyInput(false);
    browserInput.enableMouseInput(false);

    HisenseOpera& opera = browser().getHisenseOpera();

    //Send message to DMP, In order to release resouce of player
    if (m_runType == HISENSE) {
        Json::Value o;
        o["msg"] = "closeDMP";
        Json::FastWriter write;
        opera.sendPlatformEvent(m_main_window, write.write(o));
        usleep(500 * 1000);
    }

    // Will remove all event listener from store
    opera.pause();
    opera.removeAllEventListeners();
    browser().getVirtualKeyboard().leave();

#if 0
    opera.destroyHTMLView(m_status_html_view);
#else
    opera.destroyWindow(m_status_window);
#endif

    opera.destroyWindow(m_main_window);

    browserInput.setEventsIntercept(NULL);
    Graphics& graphics = browser().getGraphics();
    graphics.lostFocus();
}

void BrowserModeStore::setUrl(std::string url)
{
    BROWSER_LOG(ALWAYS, "url = %s", url.c_str());
    bool tts_enabled = browser().getConfiguration().getTTSEnable();
    HisenseOpera& opera = browser().getHisenseOpera();
    if(tts_enabled && filterUrl(url)) {
        opera.setPref(m_main_window, "chromevox_enabled.override", "true");
    }
    else {
        opera.setPref(m_main_window, "chromevox_enabled.override", "false");
    }
    std::string highlight_enabled =browser().getConfiguration().getValueByKey("TTSHighlightEnabled");
    if(highlight_enabled.empty()){
        highlight_enabled = "false";
    }
    opera.setPref(m_main_window, "accessibility_focus_highlight_enabled", highlight_enabled);
    opera.setPref(m_main_window, "accessibility_cursor_highlight_enabled", highlight_enabled);
    opera.setPref(m_main_window, "accessibility_caret_highlight_enabled", highlight_enabled);
    //opera.show(m_main_window);
    //opera.focus( m_main_window );
    //m_runType = BrowserUtil::m_storeType;
    BROWSER_LOG(ALWAYS, "m_runType is %d", m_runType);
    std::string netrangeUrl = opera.getNetranageUrl();
    std::string foxxumUrl = opera.getFoxxumUrl();
    if (url.empty()&&(m_runType == NETRANGE)) {
        url = netrangeUrl;
        if (!checkNetwork(url)) {
            opera.clearDiskCache();
        }
    } else if (url.empty()&&(m_runType == FOXXUM)) {
        url = foxxumUrl;
    } else if (m_runType == ACCEDO || url == "http://appsol.accedo.tv/hisense/matrix.html") {
        BROWSER_LOG(INFO, "Reset the gamecenter resolution");
        opera.setPref(m_main_window, "content_resolution", "1920x1080");
    } else {
        BROWSER_LOG(INFO, "the default url = %s", url.c_str());
    }
    std::size_t found = url.find("html5.toongoggles");
    if (found != std::string::npos) {
        BROWSER_LOG(ERROR, "Current app is HTML5 TOONGOGGLES");
        opera.clearApplicationCache();
        opera.clearDiskCache();
    }
    std::size_t found1 = url.find("apps.showmax.com");
    if (found1 != std::string::npos) {
        BROWSER_LOG(ERROR, "Current app is showmax");
        opera.setPref(m_main_window, "spatial_navigation_enabled", "false");
    }
    BROWSER_LOG(INFO, "url = %s", url.c_str());
    m_current_url = url;
    opera.loadURL(m_main_window, url);
    if(url.find("epos.html") == std::string::npos){
        waitStatusWindowFinished(); //notify to APM appstarted after apploading circle diaplay
    }
    browser().appStarted(BROWSER_MODE_STORE, url);

}
void BrowserModeStore::waitStatusWindowFinished()
{
    m_status_finished = false;
    int count = 0;
    while(!m_status_finished){
        if(count < 45){
            BLOGI("waiting for status_window loadfinish.");
            usleep(200*1000);
            count++;
        } else {
            BLOGI("timeout,break");
            break;
        }
    }
}
void BrowserModeStore::processOOM()
{
    if (!m_OOMEntered) {
        HisenseOpera& opera = browser().getHisenseOpera();
        opera.stopLoading(m_main_window);
#if 0 //def HTML_IMPLEMENT
        opera.hide(m_status_html_view);
        opera.setStackAtBottom(m_status_html_view);
#else
        opera.hide(m_status_window);
        opera.setStackAtBottom(m_status_window);
#endif
        opera.loadURL(m_main_window, m_oom_url);
        opera.focus(m_main_window);
        m_OOMEntered = true;
    }
}

void BrowserModeStore::loadingStarted(opera::evt::LoadingStarted event)
{
    BROWSER_LOG(ALWAYS, "Loading started Handle: %s %s", event.window_id.str().c_str(), event.url.c_str());
    HisenseOpera& opera = browser().getHisenseOpera();
    // HTML view loading started on the parent window
    if (event.window_id == m_main_window) {
        BROWSER_LOG(ALWAYS, "m_main_window loading start.");
#if 0
        opera.show(m_status_html_view);
#else
        if(event.url.find("epos.html") == std::string::npos){
            opera.show( m_status_window );
        }

#endif
        opera.show(m_main_window);
        opera.focus(m_main_window);

    }
}

void BrowserModeStore::loadingFinished(opera::evt::LoadingFinished event)
{
    HisenseOpera& opera = browser().getHisenseOpera();
    std::string statusPageUrl = opera.getStatusPageUrl();
    std::string errorPage = opera.getErrorPage();
    BROWSER_LOG(INFO, "statusPageUrl : %s", statusPageUrl.c_str());
    BROWSER_LOG(INFO, "errorPage : %s", errorPage.c_str());
    std::string status_str;

    if (event.window_id == m_status_window) {
        m_status_finished = true;
    }

    switch (event.status)
    {
        case opera::evt::LoadingFinished::LOAD_SUCCEEDED:
            // HTML view loading finished on the parent window
            ///TODO:fliter some url
            if (event.window_id == m_main_window) {
                BROWSER_LOG(INFO, "m_main_window loading finished.");
#if 0 //def HTML_IMPLEMENT
                opera.hide(m_status_html_view);
#else
                if(event.url.find("epos.html") == std::string::npos){
                    opera.hide(m_status_window);
                }
#endif
                opera.focus(m_main_window);
            }
            status_str = "succeeded";
            break;
        case opera::evt::LoadingFinished::LOAD_FAILED:
#if 0
            opera.hide(m_status_html_view);
#else
        if(event.url.find("epos.html") == std::string::npos){
            opera.hide(m_status_window);
        }
#endif
            opera.focus(m_main_window);
            opera.loadURL(m_main_window, errorPage);
            status_str = "failed";
            break;
        case opera::evt::LoadingFinished::LOAD_ABORTED:
#if 0
            opera.hide(m_status_html_view);
#else
        if(event.url.find("epos.html") == std::string::npos){
            opera.hide(m_status_window);
        }
#endif
            opera.focus(m_main_window);

            status_str = "aborted";
            break;
        default:
            status_str = "other";
    }

    BROWSER_LOG(ALWAYS, "Loading finished (%s) window: %s %s", status_str.c_str(), event.window_id.str().c_str(), event.url.c_str());
}

void BrowserModeStore::certificateError(opera::evt::CertificateError event)
{
    BROWSER_LOG(ALWAYS, "Certificate error window: %s", event.window_id.str().c_str());
    BROWSER_LOG(ALWAYS, "Certificate error certificate_error: %d", event.ssl_info.certificate_error);
    BROWSER_LOG(ALWAYS, "Certificate error request url: %s", event.ssl_info.request_url.c_str());

    BROWSER_LOG(ALWAYS, "Certificate error overridable: %d", event.ssl_info.overridable);
    BROWSER_LOG(ALWAYS, "Certificate error strict_enforcement: %d", event.ssl_info.strict_enforcement);
    BROWSER_LOG(ALWAYS, "Certificate error is_issued_by_known_root: %d", event.ssl_info.is_issued_by_known_root);
    BROWSER_LOG(ALWAYS, "Certificate error client_cert_sent: %d", event.ssl_info.client_cert_sent);
    BROWSER_LOG(ALWAYS, "Certificate error channel_id_sent: %d", event.ssl_info.channel_id_sent);
    BROWSER_LOG(ALWAYS, "Certificate error security_bits: %d", event.ssl_info.security_bits);
    BROWSER_LOG(ALWAYS, "Certificate error ciphersuite_id: %d", event.ssl_info.ciphersuite_id);
    BROWSER_LOG(ALWAYS, "Certificate error ssl_connection_version: %d", event.ssl_info.ssl_connection_version);
    BROWSER_LOG(ALWAYS, "Certificate error handshake_type: %d", event.ssl_info.handshake_type);
    ///TODO:White list???
    if (event.ssl_info.overridable) {
        BROWSER_LOG(ALWAYS, "Allow the connection");
        event.Allow();
    }
}

void BrowserModeStore::ScriptRequestCloseWindow(opera::evt::ScriptRequestCloseWindow event)
{
    BROWSER_LOG(INFO, "Got ScriptRequestCloseWindow window_id %s opened_by_dom %d url %s",
                event.id.str().c_str(), event.opened_by_dom, event.url.c_str());

    browser().getTaskManager().postTask(hisense::AsyncBrowserTask::create(&browser(), "pause"));
}

void BrowserModeStore::onPlatformMessage(opera::evt::PlatformMessage event)
{
    if (event.message == "exitCurrentMode") {
        browser().getTaskManager().postTask(hisense::AsyncBrowserTask::create(&browser(), "pause"));
        return;
    }
    if (event.message == "reload_app") {
        HisenseOpera& opera = browser().getHisenseOpera();
        opera.loadURL(m_main_window, m_current_url);
        return;
    }
    //Send other message to browser:
    browser().appMessageArrived(BROWSER_MODE_STORE, event.message);



}

void BrowserModeStore::sendMessage(const std::string & message)
{ //TODO: Notify message to web pages using sendPlatformEvent
    HisenseOpera& opera = browser().getHisenseOpera();
    opera.sendPlatformEvent(m_main_window, message);
}

bool BrowserModeStore::checkNetwork(std::string url)
{
    bool result = false;
#if 0
    std::string checkUrl = url;
    checkUrl = "busybox wget " + checkUrl + " -O /tmp/rash.tmp";
#else
    std::string checkUrl = getHostName(url);
    checkUrl = "ping -c 1 -i 1 -w 1 " + checkUrl;
#endif
    int ret = system(checkUrl.c_str());
    if (WIFEXITED(ret)) {
        if (0 == WEXITSTATUS(ret)) {
            result = true;
        }
    }
    return result;
}

std::string BrowserModeStore::getHostName(std::string url)
{
    std::size_t start = url.find("://");
    if (start != std::string::npos) {
        url = url.substr(start + 3);
    }
    std::size_t end = url.find("/");
    if (end != std::string::npos) {
        url = url.substr(0, end);
    }
    return url;
}

bool BrowserModeStore::filterUrl(std::string url)
{
    if(url.find("viewster.tv") == std::string::npos) {
        return true;
    }
    else {
        return false;
    }
}

