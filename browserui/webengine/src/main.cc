#include <unistd.h>
#include <string>
#include "browser-impl.h"
#include "configuration.h"
#include "browser_debug.h"
#include "hisense_webengine.h"
#include "base/log/browser_log.h"
#include "browser-input.h"
#include "base/utils/properties.h"

namespace {
static BrowserImpl * g_brower_impl = 0;

typedef struct
{
    int mode;
    std::string url;
    std::string type;
    bool isRequested;
    void reset() { mode=0;url="";type="";isRequested=false;}
} DeferEvent;
DeferEvent g_defer_event;

void createWebEngine(int argc, char*argv[])
{
    if (g_brower_impl) {
        delete g_brower_impl;
    }
    g_brower_impl = new BrowserImpl(argc, argv);

}

BrowserImpl& browserInstance()
{
    return *g_brower_impl;
}
}

static int browserModeToWebEngineMode(BrowserModeType mode)
{
    //The BROWSER_MODE_TYPE_MAX is a valid value:

    if (mode > BROWSER_MODE_TYPE_MAX) {
        BLOGE("Unknown Browser Mode: %d", mode);
        return WE_MODE_OFF;
    }

    static int mode_map[BROWSER_MODE_TYPE_MAX - BROWSER_MODE_OFF + 1] = {0};

    static bool map_inited = false;

    if (!map_inited) {
        mode_map[BROWSER_MODE_OFF - BROWSER_MODE_OFF] = WE_MODE_OFF;
        mode_map[BROWSER_MODE_HBBTV - BROWSER_MODE_OFF] = WE_MODE_HBBTV;
        mode_map[BROWSER_MODE_STORE - BROWSER_MODE_OFF] = WE_MODE_STORE;
        mode_map[BROWSER_MODE_OPERA_STORE - BROWSER_MODE_OFF] = WE_MODE_OPERA_STORE;
        mode_map[BROWSER_MODE_HIBROWSER - BROWSER_MODE_OFF] = WE_MODE_BROWSER;
        mode_map[BROWSER_MODE_OIPF - BROWSER_MODE_OFF] = WE_MODE_OIPF;
        mode_map[BROWSER_MODE_YOUTUBE - BROWSER_MODE_OFF] = WE_MODE_YOUTUBE;
        map_inited = true;
    }

    return mode_map[mode - BROWSER_MODE_OFF];
}

static BrowserModeType webEngineModeToBrowserMode(int mode)
{
    if (mode - WE_MODE_OFF >= WE_MODE_COUNT) {
        BLOGE("Unknown WebEngine Mode: %d", mode);
        return BROWSER_MODE_OFF;
    }
    static BrowserModeType mode_map[WE_MODE_COUNT] = {BROWSER_MODE_OFF};
    static bool map_inited = false;

    if (!map_inited) {
        mode_map[WE_MODE_OFF - WE_MODE_OFF] = BROWSER_MODE_OFF;
        mode_map[WE_MODE_HBBTV - WE_MODE_OFF] = BROWSER_MODE_HBBTV;
        mode_map[WE_MODE_STORE - WE_MODE_OFF] = BROWSER_MODE_STORE;
        mode_map[WE_MODE_OPERA_STORE - WE_MODE_OFF] = BROWSER_MODE_OPERA_STORE;
        mode_map[WE_MODE_BROWSER - WE_MODE_OFF] = BROWSER_MODE_HIBROWSER;
        mode_map[WE_MODE_OIPF - WE_MODE_OFF] = BROWSER_MODE_OIPF;
        mode_map[WE_MODE_YOUTUBE - WE_MODE_OFF] = BROWSER_MODE_YOUTUBE;
        map_inited = true;
    }
    return mode_map[mode - WE_MODE_OFF];
}

static std::string modeTypeToModeName(BrowserModeType mode)
{
    switch (mode)
    {
        case BROWSER_MODE_OFF:
            return "OFF";
        case BROWSER_MODE_STORE:
            return "STORE";
        case BROWSER_MODE_OPERA_STORE:
            return "OPERA_STORE";
        case BROWSER_MODE_HBBTV:
            return "HBBTV";
        case BROWSER_MODE_HIBROWSER:
            return "HIBROWSER";
        case BROWSER_MODE_OIPF:
            return "OIPF";
        case BROWSER_MODE_YOUTUBE:
            return "YOUTUBE";
        default:
            return "UNKNOWN";

    }
}

static BrowserModeType modeNameToModeType(const std::string & mode)
{
    BrowserModeType mode_id = BROWSER_MODE_OFF;

    if (mode == "OFF") {
        mode_id = BROWSER_MODE_OFF;
    } else if (mode == "STORE") {
        mode_id = BROWSER_MODE_STORE;
    } else if (mode == "OPERA_STORE") {
        mode_id = BROWSER_MODE_OPERA_STORE;
    } else if (mode == "HBBTV") {
        mode_id = BROWSER_MODE_HBBTV;
    } else if (mode == "HIBROWSER") {
        mode_id = BROWSER_MODE_HIBROWSER;
    } else if (mode == "OIPF") {
        mode_id = BROWSER_MODE_OIPF;
    } else if (mode == "YOUTUBE") {
        mode_id = BROWSER_MODE_YOUTUBE;
    } else {
        mode_id = BROWSER_MODE_HBBTV;
    }
    return mode_id;
}

class WebengineEventListener : public BrowserEventListener
{
public:

    WebengineEventListener(void * ctx, EventListenerFunc* func)
    : m_ctx(ctx)
    , m_func(func)
    {

    }

    virtual ~WebengineEventListener()
    {
    }
protected:

    virtual void OnKeySetChanged(BrowserModeType appmode, unsigned int keyset) override
    {
        BLOGI("KeySetChanged: 0X%03X", keyset);
        if (m_func) {
            WeEvent event;
            event.type = KeySetChanged;
            event.param.asInt = browserModeToWebEngineMode(appmode);
            event.param1.asUint = keyset;
            m_func(m_ctx, event);
        }
    }

    virtual void OnAppStarted(BrowserModeType appmode, const std::string & appurl) override
    {
        BLOGI("AppStarted: %d,%s", appmode, appurl.c_str());
        if (m_func) {
            WeEvent event;
            event.type = APPStarted;
            event.param.asInt = browserModeToWebEngineMode(appmode);
            m_func(m_ctx, event);
        }
    }

    virtual void OnAppStopped(BrowserModeType appmode, const std::string & appurl) override
    {
        BLOGI("AppStopped: %d,%s", appmode, appurl.c_str());
        if (m_func) {
            WeEvent event;
            event.type = APPStopped;
            event.param.asInt = browserModeToWebEngineMode(appmode);
            m_func(m_ctx, event);
        }
    }

    virtual void OnAppMessage(BrowserModeType appmode, const std::string & message)
    {
        if (m_func) {
            WeEvent event;
            event.type = APPMessage;
            event.param.asInt = appmode;
            std::string msg(message);
            const char * msg1 = message.c_str();

            event.param1.asPointer = (void*) msg1;
            BLOGV("Send Message TO APM: %s", msg1);
            //FIXME: the pointer int event is not allocated,
            //it should never be used after callback:
            m_func(m_ctx, event);
        }
    }

private:
    void * m_ctx;
    EventListenerFunc * m_func;
};

static int s_webengine_is_ready = 0;
int hisense_webengine_start(int argc, char* argv[], EventListenerFunc cb, void * ctx)
{
    BLOGS("Starting Webengine...");

    s_webengine_is_ready = 0;
    createWebEngine(argc, argv);

    BrowserImpl& browser = browserInstance();

    //listener will be owned by browser:
    browser.setEventListener(new WebengineEventListener(ctx, cb));

    browser.start();

    //if the isRequested  is true means someapp requeststart when webengine is not ready.
    //Resume it when webengine started.
    BROWSER_LOG(INFO, "event.deferflag: %d", g_defer_event.isRequested);
    if (g_defer_event.isRequested) {
        g_defer_event.isRequested = false;
        s_webengine_is_ready = 1;
        hisense_webengine_load_url(g_defer_event.mode, g_defer_event.url.c_str(), g_defer_event.type.c_str());
    }else{
        s_webengine_is_ready = 1;
    }
    /*else {

        Configuration &config = browser.getConfiguration();
        std::string url = config.getValueByKey("BrowserStartUrl");
        BROWSER_LOG(ALWAYS, "Home URL: %s", url.c_str());

        std::string mode = config.getValueByKey("BrowserStartMode");
        BROWSER_LOG(ALWAYS, "Home Mode: %s", mode.c_str());
        BrowserModeType mode_id = modeNameToModeType(mode);
        browser.setUrl(mode_id, url);
        base::set_process_property(PROPERTY_BROWSER_CURRENT_MODE, mode.c_str());
        if (g_defer_event.isRequested) {
            g_defer_event.isRequested = false;
            s_webengine_is_ready = 1;
            hisense_webengine_load_url(g_defer_event.mode, g_defer_event.url.c_str(), g_defer_event.type);
        } else {

            s_webengine_is_ready = 1;
        }
    }*/
    return 0;
}

int hisense_webengine_cancel_pending_request()
{
     if (s_webengine_is_ready != 1){
        BLOGI("Set defer_event to null when STRsuspend.");
        g_defer_event.reset();
        return -1;
     }
     return 0;
}

int hisense_webengine_load_url(int mode, const char * url, std::string type)
{
    BrowserImpl& browser = browserInstance();
    if (s_webengine_is_ready != 1) {
        BLOGE("Webengine is not ready yet!");
        //1.Last command is to start the App that except HBBTV;
        //2.Current command is to suspend it;
        //The defer should be clean up,and return stoped to APM.
        if((WE_MODE_OFF != g_defer_event.mode)
            &&(WE_MODE_HBBTV != g_defer_event.mode)
            &&(WE_MODE_OFF == mode)){
            BrowserModeType modeType = BROWSER_MODE_OFF;
            std::string modeName = "BROWSER_MODE_OFF";

            modeType = webEngineModeToBrowserMode(g_defer_event.mode);
            modeName = modeTypeToModeName(modeType);
            BROWSER_LOG(ALWAYS, "Stop APP %s when the webengine is not ready!",modeName.c_str());
            g_defer_event.reset();
            browser.appStopped(modeType, "");
            return -1;
        }
        g_defer_event.mode = mode;
        g_defer_event.url = url;
        g_defer_event.type = type;
        g_defer_event.isRequested = true;
        return -1;
    }
    BrowserModeType mode_type = BROWSER_MODE_OFF;
    std::string mode_name = "BROWSER_MODE_OFF";
    AppType submode = OTHER;
    if (type == "foxxum") {
        submode = FOXXUM;
    } else if (type == "netrange") {
        submode = NETRANGE;
    } else if (type == "hisense") {
        submode = HISENSE;
    } else if (type == "accedo") {
        submode = ACCEDO;
    } else {
        submode = OTHER;
    }
    BROWSER_LOG(INFO, "The submode is %s, %d", type.c_str(), submode);

    mode_type = webEngineModeToBrowserMode(mode);
    mode_name = modeTypeToModeName(mode_type);
    MultiModeArgument modeArgument = {submode};

    BROWSER_LOG(INFO, "Loading URL in %s mode (%s)", mode_name.c_str(), url);

    browser.setUrl(mode_type, url, &modeArgument);
    base::set_process_property(PROPERTY_BROWSER_CURRENT_MODE, mode_name.c_str());

    return 0;
}

/**
 * return 0 means eat the input
 * return 1 means the input is ignored
 */
int hisense_webengine_inject_keyevent(int we_mode, unsigned int type, unsigned symbol, unsigned int id, unsigned int modifier)
{
    if (s_webengine_is_ready != 1) {
        BLOGE("Webengine is not ready yet!");
        return -1;
    }

    BrowserImpl& browser = browserInstance();
    BrowserModeType current_mode = browser.currentMode();
    BrowserModeType key_mode = webEngineModeToBrowserMode(we_mode);
    std::string modename = base::get_process_property(PROPERTY_BROWSER_CURRENT_MODE);
    BLOGV("Current BrowserMode: current:%s,property:%s", ToString(current_mode).c_str(), modename.c_str());


    //If HBBTV is runnning and TTX or MHEG is also running,
    //Just ignore events and passing events to TTX/MHEG
    if (browser.isInStealthMode()) {
        BLOGI("Browser is In Stealth Mode, ignore any keys");
        return -3;
    }
    if (current_mode == BrowserModeType::BROWSER_MODE_OFF) {
        BLOGI("Browser is In OFF Mode, ignore any keys");
        return -3;
    }

    BrowserInput& input = browser.getBrowserInput();
    int result = input.generateKeyEvent(type, symbol, id, modifier);

    return (result == 0) ? 0 : 1;
}

int hisense_webengine_send_message(int we_mode, const char* src, const char * message)
{
    if (s_webengine_is_ready != 1) {
        BLOGE("Webengine is not ready yet!");
        return -1;
    }

    BrowserImpl& browser = browserInstance();
    BrowserModeType current_mode = browser.currentMode();
    BrowserModeType target_mode;
    //FIXME: we use off mode to represent current one:
    if (we_mode == WE_MODE_OFF) {
        target_mode = current_mode;
    } else {
        target_mode = webEngineModeToBrowserMode(we_mode);
    }

    if (target_mode != current_mode) {
        BLOGW("Mode (%d) is not Current(%d)");
        return -1;
    }
    std::string msg = message;
    browser.notifyApp(target_mode, msg);

}
