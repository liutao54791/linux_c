#include <fstream>
#include <limits.h>
#include <algorithm>

//#include <opera/components/broadcast/broadcast_integration.h>
#include <opera/messages/CreateHbbTVWindow.h>
#include <opera/messages/KeyInput.h>
#include <uva/utils/log.h>
#include "graphics.h"
#include "configuration.h"
#include "browser-opera.h"
#include "browser_debug.h"
#include "browser-input.h"
#include "browser-mode-hbbtv.h"
#include "virtual-keyboard.h"
#include "NotImplemented.h"
#include "systeminfo.h"
#include "base/utils/properties.h"

BrowserModeHBBTV::InputInterceptor::InputInterceptor(BrowserImpl* browserImpl, BrowserModeHBBTV* parent_hbbtv_mode)
: m_browserImpl(browserImpl), parent_hbbtv_mode_(parent_hbbtv_mode)
{
}

bool BrowserModeHBBTV::InputInterceptor::WouldConsume(
        const opera::msg::KeyInput& key_input)
{
    if (opera::msg::KeyInput::OMI_KEY_POWER == key_input.virtual_key) {
        if (opera::msg::KeyInput::KEYDOWN == key_input.event_type) {
            browser().setUrl(BROWSER_MODE_OFF, "");
        }
        BLOGW("Stop Browser By Power Button.");
        return false;
    }
    if (key_input.virtual_key == opera::msg::KeyInput::OMI_KEY_TELETEXT) {
        BLOGV("Always accept TTX key ");
        return true;
    }
    int keyset = parent_hbbtv_mode_->GetCurrentKeyset();
    opera::msg::KeyInput::Key vkey = key_input.virtual_key;
    BLOGV("Keyset: %x, vkey : %x(%d)", keyset, vkey, vkey);
    return parent_hbbtv_mode_->InKeyset(vkey, keyset);
}

bool BrowserModeHBBTV::InputInterceptor::InterceptKeyEvent(
        const opera::msg::KeyInput& key_input)
{
    if (key_input.event_type == opera::msg::KeyInput::KEYDOWN) {
        switch (key_input.virtual_key)
        {
        case opera::msg::KeyInput::OMI_KEY_POWER:
        case opera::msg::KeyInput::OMI_KEY_F12:
        {
            /// TODO:leave Browser mode, enter OFF mode
            browser().setUrl(BROWSER_MODE_OFF, "");
            return true;
        }
        case opera::msg::KeyInput::OMI_KEY_BACKSPACE:
        {
            opera::msg::KeyInput key_input_converted = key_input;
            key_input_converted.virtual_key = opera::msg::KeyInput::OMI_KEY_BACK;
            browser().getHisenseOpera().sendKeyInput(key_input_converted);
            return true;
        }
        case opera::msg::KeyInput::OMI_KEY_ESCAPE:
        {
            BROWSER_LOG(ERROR, "HBBTV receive the key of OMI_KEY_ESCAPE\n");
            parent_hbbtv_mode_->pause();
            //need to resume when in liveTv.
            parent_hbbtv_mode_->resume();
            return true;
        }
        case opera::msg::KeyInput::OMI_KEY_TELETEXT:
        {

            parent_hbbtv_mode_->onTTXKeyPressed();
            return true;
        }
        default:
        {
            break;
        }
        }
    } else if (key_input.event_type == opera::msg::KeyInput::KEYUP) {
        switch (key_input.virtual_key)
        {
        case opera::msg::KeyInput::OMI_KEY_BACKSPACE:
        {
            opera::msg::KeyInput key_input_converted = key_input;
            key_input_converted.virtual_key = opera::msg::KeyInput::OMI_KEY_BACK;
            browser().getHisenseOpera().sendKeyInput(key_input_converted);
            return true;
        }
        default:
        {
            break;
        }
        }
    }
    return false;
}

BrowserModeHBBTV::BrowserModeHBBTV(BrowserImpl* browserImpl)
: BrowserMode(browserImpl)
, m_input_interceptor(NULL)
{

    TRACE_HERE();
}

BrowserModeHBBTV::~BrowserModeHBBTV()
{

    TRACE_HERE();

    if (m_input_interceptor) {
        delete m_input_interceptor;
        m_input_interceptor = NULL;
    }
}

namespace {

std::string RealPath(const std::string& path)
{
    char buf[PATH_MAX];
    if (realpath(path.c_str(), buf)) return buf;
    return path;
}

} // namespace

BrowserStatus BrowserModeHBBTV::start()
{
    BROWSER_LOG(ALWAYS, "HBBTV mode start");

    std::vector<opera::msg::CreateHbbTVWindow::PlatformProfile> platform_profiles{opera::msg::CreateHbbTVWindow::FREEVIEW_2_0};

    hbbtv_window_ = CreateHbbTVWindow(platform_profiles);

    OverrideFallbackFonts();

    HisenseOpera& hisense_opera = browser().getHisenseOpera();
    BLOGV("Enable XHR Cross Origin Access");
    hisense_opera.setPref(hbbtv_window_, "xhr_origin_check_enabled", "false");
    hisense_opera.setPref(hbbtv_window_, "block_mixed_xhr_content", "false");

    // notify js of videocontrol not work
    hisense_opera.sendPlatformEvent(hbbtv_window_, "stop-videocontrol");

    m_input_interceptor = new InputInterceptor(&browser(), this);
    if (!m_input_interceptor) {
        LOG(LEVEL_ERR, "Failed to create input interceptor\n");
        return BrowserError;
    }

    opera::components::OperaController& opera_controller =
            *(hisense_opera.getOperaController());

    opera::components::CommandLine& commadLine =
            browser().getConfiguration().getCommandLine();

    std::string ait_file_name = commadLine.HasOption("ait")
            ? RealPath(commadLine.GetOptionValue("ait"))
            : "";
    std::string mock_dsmcc_path =
            commadLine.HasOption("dsmcc")
            ? RealPath(commadLine.GetOptionValue("dsmcc"))
            : "";

    hbbtv_controller_ptr_.reset(new browser::hbbtv::HbbTVController(this,
                                                                    opera_controller, hbbtv_window_, platform_profiles, ait_file_name,
                                                                    mock_dsmcc_path));
    hbbtv_controller_ptr_->Start();

    m_HbbTV.reset(new browser::hbbtv::HbbTVIntegration(this));
    m_HbbTV->start();

    return BrowserOK;
}

opera::components::OperaController* BrowserModeHBBTV::getOperaController()
{
    HisenseOpera& hisense_opera = browser().getHisenseOpera();
    return hisense_opera.getOperaController();
}

opera::Handle BrowserModeHBBTV::getCurrentHbbTVWindow()
{
    return hbbtv_window_;
}

void BrowserModeHBBTV::stop()
{
    BROWSER_LOG(ERROR, "HBBTV stop");
    HisenseOpera& opera = browser().getHisenseOpera();
    if (hbbtv_window_.IsValid()) opera.destroyWindow(hbbtv_window_);

    if (m_input_interceptor) {
        delete m_input_interceptor;
        m_input_interceptor = NULL;
    }

    if (!hbbtv_controller_ptr_) {
        LOG(LEVEL_WARN, "hbbtv controller not started yet, or something bad happened\n");
        return;
    }

    hbbtv_controller_ptr_->Stop();
    if (m_HbbTV) {
        m_HbbTV->stop();
    }
}

void BrowserModeHBBTV::enter(MultiModeArgument *argument)
{
    BLOGA("HBBTV Enter");
    Graphics& graphics = browser().getGraphics();
    graphics.requestFocus(false, true);

    BrowserInput& browserInput = browser().getBrowserInput();
    // Disable physical mouse input and enable key input
    browserInput.enableKeyInput(true);
    browserInput.enableMouseInput(false);

    HisenseOpera& opera = browser().getHisenseOpera();

    opera.show(hbbtv_window_);
    opera.focus(hbbtv_window_);

    // HBBTV window don't support create HTML view
    // enter virtual keyboard
    // browser().getVirtualKeyboard().enter( m_main_window );

    // Set input interceptor
    if (m_input_interceptor)
        browserInput.setEventsIntercept(m_input_interceptor);

    registerEventListers();

    //   opera.AddEventHandler( &BrowserModeHBBTV::loadingStarted, this);
    //    opera.AddEventHandler( &BrowserModeHBBTV::onLoadingProgress, this);
    //   opera.AddEventHandler( &BrowserModeHBBTV::loadingFinished, this);
    opera.AddEventHandler(&BrowserModeHBBTV::ParentalControlQuery, this);
    opera.resume();
    if (hbbtv_controller_ptr_) {
        hbbtv_controller_ptr_->Resume();
    }
    if (m_HbbTV) {
        m_HbbTV->run();
    }

    applicationStarted("");
    base::set_process_property(PROPERTY_BROWSER_HBBTV_APPLICATION_STRSUSPEND, "false");
}

void BrowserModeHBBTV::leave()
{
    BLOGA("HBBTV Leave");

    BrowserInput& browserInput = browser().getBrowserInput();
    // Disable physical mouse input and key input
    browserInput.enableKeyInput(false);
    browserInput.enableMouseInput(false);

    /// TODO:clear the HBBTV application vector?
    HisenseOpera& opera = browser().getHisenseOpera();
    // Will remove all event listener of hbbtv
    opera.pause();
    opera.removeAllEventListeners();

    // leave virtual keyboard
    // browser().getVirtualKeyboard().leave();

    opera.terminateApplication(hbbtv_window_, false);
    opera.hide(hbbtv_window_);
    // Set input interceptor to NULL
    browserInput.setEventsIntercept(NULL);
    setTVVideoWindowToFullScreenIfNeeded();
    hbbtv_controller_ptr_->Pause();
    if (m_HbbTV) {
        m_HbbTV->pause();
    }
    updateKeySet(0);
    Graphics& graphics = browser().getGraphics();
    graphics.lostFocus();

    applicationStopped("");

    base::set_process_property(PROPERTY_BROWSER_HBBTV_APPLICATION_STOPED, "true");

    std::string is_STRsuspend = base::get_process_property(PROPERTY_BROWSER_HBBTV_APPLICATION_STRSUSPEND);

    BLOGI("is_STRsuspend: %s", is_STRsuspend.c_str());
    if (is_STRsuspend == "false") {
        BLOGA("Sending Resume DTV Request");
        base::set_process_property(PROPERTY_BROWSER_HAS_RESUME_BROADCAST_REQUEST, "true");
    }

}

void BrowserModeHBBTV::onTTXKeyPressed()
{
    if (hbbtv_controller_ptr_) {
        hbbtv_controller_ptr_->onTTXKeyPressed();
    }
}
//void BrowserModeHBBTV::onLoadingProgress( opera::evt::LoadingProgress event)
//{
//    BROWSER_LOG(ERROR, "HBBTV onLoadingProgress");
//}

void BrowserModeHBBTV::updateKeySet(int keyset)
{
    BLOGI("new Keyset: %d(0x%X)", keyset, keyset);
    BrowserInput& browserInput = browser().getBrowserInput();
    browserInput.updateKeySet(BROWSER_MODE_HBBTV, keyset);
    if (keyset & 0xFFFFFFF0) {
        base::set_process_property(PROPERTY_BROWSER_HBBTV_KEYSET_HAVE_NAVIGATION, "true");
    } else {
        base::set_process_property(PROPERTY_BROWSER_HBBTV_KEYSET_HAVE_NAVIGATION, "false");
    }
    browser().getConfiguration().hbbtvOSDStatusChange();
}

void BrowserModeHBBTV::applicationStarted(const std::string & appurl)
{
    browser().appStarted(BROWSER_MODE_HBBTV, appurl);
}

void BrowserModeHBBTV::applicationStopped(const std::string & appurl)
{
    browser().appStopped(BROWSER_MODE_HBBTV, appurl);
}

void BrowserModeHBBTV::updateDTVTTXStatus(int status)
{
    TRACE_HERE();
    //If DTVTTX is running , we need to enter stealth mode
    //Otherwise we leave stealth mode.
    bool should_be_stealth = (status == 0) ? false : true;

    bool old_stealth_mode = browser().isInStealthMode();

    browser().updateStealthStatus(should_be_stealth);

    //If TTX status changed, we should pause/resume HbbTV:

    if (old_stealth_mode != should_be_stealth) {
        if (should_be_stealth) {
            this->pause();
        } else {
            this->resume();
        }
    }

}

int BrowserModeHBBTV::getCurrentBrowserMode()
{
    return browser().currentMode();
}

void BrowserModeHBBTV::ParentalControlQuery(opera::evt::ParentalControlQueryEvent event)
{
    BLOGI("HBBTV ParentalControlQuery");

    int control_rating = -1;
    int systrem_rating = -1;

    control_rating = ParseParentalControlRating(event.query_data);
    if (control_rating < 0) {
        BLOGE("control_rating is %d\n", control_rating);
        return;
    }

    systrem_rating = GetSystemParentalRating();
    if (systrem_rating < 0) {
        BLOGE("systrem_rating is %d\n", systrem_rating);
        return;
    }

    BLOGI("control_rating = %d, systrem_rating = %d\n", control_rating, systrem_rating);

    if (!isAllowtoPlayback(control_rating, systrem_rating)) {
        BLOGI("ParentalControl Deny\n");
        event.Deny();
    }

    return;
}

int BrowserModeHBBTV::ParseParentalControlRating(const std::string& query_data)
{
    if (query_data.find("mpeg7:ParentalRating") == std::string::npos) {
        BLOGE("can not get ParentalRating\n");
        return -1;
    }
    unsigned int pos_href = query_data.find("href");
    if (pos_href == std::string::npos) {
        BLOGE("can not get rating value.\n");
        return -1;
    }
    unsigned int position_1 = query_data.find_first_of("\"", pos_href);
    unsigned int position_2 = query_data.find_first_of("\"", position_1 + 1);
    std::string url = query_data.substr(position_1, position_2 - position_1);
    BLOGI("The contents of href is:%s", url.c_str());

    unsigned int pos_rating = url.find_last_of(":");
    if (pos_rating == std::string::npos) {
        BLOGE("can not get rating value.\n");
        return -1;
    }

    std::string rating;
    rating.assign(url, pos_rating + 1, url.length() - 1);

    return atoi(rating.c_str());

}

int BrowserModeHBBTV::GetSystemParentalRating()
{

    int parental_age = browser().getConfiguration().getParentalRatingvalue();
    return parental_age;
}

bool BrowserModeHBBTV::isAllowtoPlayback(int control_rating, int systrem_rating)
{
    if ((systrem_rating > 0) && (control_rating > 0)&&(control_rating > systrem_rating)) {
        return false;
    }
    return true;
}

void BrowserModeHBBTV::pause()
{
    BLOGI("HBBTV pause called");
    if (!hbbtv_controller_ptr_) {
        BLOGW("hbbtv controller not started yet, or something bad happened\n");
        return;
    }
    hbbtv_controller_ptr_->terminateApplication();
    hbbtv_controller_ptr_->Pause();
}

void BrowserModeHBBTV::resume()
{
    if (!hbbtv_controller_ptr_) {
        LOG(LEVEL_WARN, "hbbtv controller not started yet, or something bad happened\n");
        return;
    }
    hbbtv_controller_ptr_->Resume();
}

void BrowserModeHBBTV::setUrl(std::string url)
{
    LOG(LEVEL_INFO, "url : %s\n", url.c_str());
    if (url.empty()) {
        resume();
        return;
    }
    if (!hbbtv_controller_ptr_) {
        LOG(LEVEL_WARN, "hbbtv controller not started yet, or something bad happened\n");
        return;
    }

    int scheme_len = url.find_first_of(':');

    if (scheme_len == std::string::npos) {
        BLOGW("Loading Unknown URL");
        resume();
        return;
    }

    std::string scheme = url.substr(0, scheme_len);

    if (scheme == "AIT" || scheme == "ait") {
        std::string ait_url = url.substr(scheme_len + 1);
        BLOGI("Load Remote AIT: %s", ait_url.c_str());
        injectXmlAit(ait_url);
        return;
    }

    if (scheme == "BR" || scheme == "br") {

        std::string temp_str = url.substr(scheme_len + 1);
        int org_id_len = temp_str.find_first_of(":");
        std::string org_id = temp_str.substr(0, org_id_len);
        std::string app_id = temp_str.substr(org_id_len + 1);

        BLOGI("Load  BR Application: org_id:%s,app_id:%s",
              org_id.c_str(),
              app_id.c_str());
        LoadBRApplication(org_id, app_id);
        return;
    }

    hbbtv_controller_ptr_->LoadBIApplicationNonBlocking(url);

}

void BrowserModeHBBTV::LoadBRApplication(const std::string &orgId, const std::string &appId)
{
    int org_id = strtol(orgId.c_str(), 0, 0);
    int app_id = strtol(appId.c_str(), 0, 0);

    browser().getHisenseOpera().loadBroadcastRelatedApplication(getCurrentHbbTVWindow(), app_id, org_id);

}

void BrowserModeHBBTV::injectXmlAit(std::string url)
{
    LOG(LEVEL_INFO, "called, url: %s\n", url.c_str());

    if (url.empty()) return;

    if (!hbbtv_controller_ptr_) {
        LOG(LEVEL_WARN, "hbbtv controller not started yet, or something bad happened\n");
        return;
    }

    hbbtv_controller_ptr_->InjectXmlAIT(url);
}

void BrowserModeHBBTV::setNetStatus(const int status)
{
    LOG(LEVEL_INFO, "called, status: %d\n", status);

    if ((status != 0)&&(status != 1)) return;

    if (!hbbtv_controller_ptr_) {
        LOG(LEVEL_WARN, "hbbtv controller not started yet, or something bad happened\n");
        return;
    }

    hbbtv_controller_ptr_->SetNetStatus(status);
}

void BrowserModeHBBTV::ReTune()
{
    LOG(LEVEL_INFO, "called\n");

    if (!hbbtv_controller_ptr_) {
        LOG(LEVEL_WARN, "hbbtv controller not started yet, or something bad happened\n");
        return;
    }

    hbbtv_controller_ptr_->Retune();
}

bool BrowserModeHBBTV::InKeyset(opera::msg::KeyInput::Key virtual_key, const int keyset)
{
    uint32_t unsigned_keyset = static_cast<uint32_t> (keyset);
    std::vector<opera::msg::KeyInput::Key> expanded;

    if (opera::msg::CreateHbbTVWindow::HBBTV_KEY_RED & unsigned_keyset) {
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_RED);
    }
    if (opera::msg::CreateHbbTVWindow::HBBTV_KEY_GREEN & unsigned_keyset) {
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_GREEN);
    }
    if (opera::msg::CreateHbbTVWindow::HBBTV_KEY_YELLOW & unsigned_keyset) {
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_YELLOW);
    }
    if (opera::msg::CreateHbbTVWindow::HBBTV_KEY_BLUE & unsigned_keyset) {
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_BLUE);
    }
    if (opera::msg::CreateHbbTVWindow::HBBTV_KEY_NAVIGATION & unsigned_keyset) {
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_UP);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_DOWN);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_LEFT);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_RIGHT);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_ENTER);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_BACK);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_BACKSPACE);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_ESCAPE);
    }
    if (opera::msg::CreateHbbTVWindow::HBBTV_KEY_VCR & unsigned_keyset) {
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_PLAY);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_PAUSE);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_STOP);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_TRACK_NEXT);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_TRACK_PREV);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_FAST_FWD);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_REWIND);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_PLAY_PAUSE);
    }
    if (opera::msg::CreateHbbTVWindow::HBBTV_KEY_SCROLL & unsigned_keyset) {
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_PAGE_UP);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_PAGE_DOWN);
    }
    if (opera::msg::CreateHbbTVWindow::HBBTV_KEY_INFO & unsigned_keyset) {
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_INFO);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_SUBTITLE);
    }
    if (opera::msg::CreateHbbTVWindow::HBBTV_KEY_NUMERIC & unsigned_keyset) {
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_0);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_1);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_2);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_3);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_4);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_5);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_6);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_7);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_8);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_9);
    }
    if (opera::msg::CreateHbbTVWindow::HBBTV_KEY_ALPHA & unsigned_keyset) {
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_A);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_B);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_C);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_D);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_E);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_F);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_G);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_H);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_I);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_J);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_K);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_L);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_M);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_N);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_O);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_P);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_Q);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_R);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_S);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_T);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_U);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_V);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_W);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_X);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_Y);
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_Z);
    }
    if (opera::msg::CreateHbbTVWindow::HBBTV_KEY_OTHER & unsigned_keyset) {
        expanded.push_back(opera::msg::KeyInput::OMI_KEY_TELETEXT);
    }
    return expanded.end() != std::find(expanded.begin(), expanded.end(), virtual_key);
}

int BrowserModeHBBTV::GetCurrentKeyset()
{
    if (!hbbtv_controller_ptr_) {
        LOG(LEVEL_WARN, "hbbtv controller not started yet, or something bad happened\n");
        return 0;
    }
    return browser().getBrowserInput().getCurrentKeyset();
}

opera::Handle BrowserModeHBBTV::CreateHbbTVWindow(
        std::vector<opera::msg::CreateHbbTVWindow::PlatformProfile> platform_profiles)
{
    //using opera::components::BroadcastIntegration;
    std::string ua_extra_capabilities = browser().getConfiguration().getTVCapibalitiesForHbbTV(); //"+DL+DRM+SYNC_SLAVE+AFS+PVR";
    std::string ua_vendor_name = browser().getConfiguration().getTVVendorName();
    std::string ua_model_name = browser().getConfiguration().getTVModelName();
    std::string ua_software_version = browser().getConfiguration().getTVSoftwareVersion();
    std::string ua_hardware_version = browser().getConfiguration().getTVHardwareVersion();
    std::string ua_family_name = browser().getConfiguration().getTVFamilyName();
    short application_profile = browser().getConfiguration().getApplicationProfileForHbbTV();
    int key_set_mask = opera::msg::CreateHbbTVWindow::HBBTV_KEY_ALL;

    return browser().getHisenseOpera().createHBBTVWindow(
                                                         ua_extra_capabilities, ua_vendor_name, ua_model_name,
                                                         ua_software_version, ua_hardware_version, ua_family_name,
                                                         application_profile, platform_profiles, key_set_mask);
}

void BrowserModeHBBTV::OverrideFallbackFonts()
{
    HisenseOpera& hisense_opera = browser().getHisenseOpera();
    hisense_opera.setPref(hbbtv_window_, "font_family.cursive", "Tiresias");
    hisense_opera.setPref(hbbtv_window_, "font_family.default", "Tiresias");
    hisense_opera.setPref(hbbtv_window_, "font_family.fantasy", "Tiresias");
    hisense_opera.setPref(hbbtv_window_, "font_family.fixed", "Tiresias");
    hisense_opera.setPref(hbbtv_window_, "font_family.sans", "Tiresias");
    hisense_opera.setPref(hbbtv_window_, "font_family.serif", "Tiresias");
}

/********************************
 *   Event Handlers:
 *********************************/

void BrowserModeHBBTV::registerEventListers()
{

    opera::components::OperaController* ctrl = getOperaController();

    if (!ctrl) return;

    ctrl->AddEventHandler(&BrowserModeHBBTV::OnAITParsingStatus, this); //(opera::evt::AITParsingStatus);
    ctrl->AddEventHandler(&BrowserModeHBBTV::OnAITUpdated, this); //(opera::evt::AITUpdated);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnAuthenticationRequest,this);//(opera::evt::AuthenticationRequest);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnBeforeURLRequest,this);//(opera::evt::BeforeURLRequest);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnBrowserGPUMemoryEvent,this);//(opera::evt::BrowserGPUMemoryEvent);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnBrowserMemoryEvent,this);//(opera::evt::BrowserMemoryEvent);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnCancelActiveAndPendingDialogs,this);//(opera::evt::CancelActiveAndPendingDialogs);
    ctrl->AddEventHandler(&BrowserModeHBBTV::OnCertificateError, this); //(opera::evt::CertificateError);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnCICAMFileRequested,this);//(opera::evt::CICAMFileRequested);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnClientCertificateSelectEvent,this);//(opera::evt::ClientCertificateSelectEvent);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnDisplaySize,this);//(opera::evt::DisplaySize);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnDSMCCFileRequested,this);//(opera::evt::DSMCCFileRequested);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnDSMCCIsSameCarousel,this);//(opera::evt::DSMCCIsSameCarousel);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnDSMCCSubscribeStreamEvent,this);//(opera::evt::DSMCCSubscribeStreamEvent);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnDSMCCUnsubscribeAllStreamEvents,this);//(opera::evt::DSMCCUnsubscribeAllStreamEvents);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnDSMCCUnsubscribeStreamEvent,this);//(opera::evt::DSMCCUnsubscribeStreamEvent);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnExitingFullscreenMode,this);//(opera::evt::ExitingFullscreenMode);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnFullscreenModeRequest,this);//(opera::evt::FullscreenModeRequest);
    ctrl->AddEventHandler(&BrowserModeHBBTV::OnHbbTVApplicationBlocked, this); //(opera::evt::HbbTVApplicationBlocked);
    ctrl->AddEventHandler(&BrowserModeHBBTV::OnHbbTVApplicationLoadTimedOut, this); //(opera::evt::HbbTVApplicationLoadTimedOut);
    ctrl->AddEventHandler(&BrowserModeHBBTV::OnHbbTVApplicationNotStarted, this); //(opera::evt::HbbTVApplicationNotStarted);
    ctrl->AddEventHandler(&BrowserModeHBBTV::OnHbbTVApplicationStarted, this); //(opera::evt::HbbTVApplicationStarted);
    ctrl->AddEventHandler(&BrowserModeHBBTV::OnHbbTVApplicationStopped, this); //(opera::evt::HbbTVApplicationStopped);
    ctrl->AddEventHandler(&BrowserModeHBBTV::OnHbbTVApplicationVisibilityChanged, this); //(opera::evt::HbbTVApplicationVisibilityChanged);
    ctrl->AddEventHandler(&BrowserModeHBBTV::OnHbbTVBroadcastRelatedStatusChanged, this); //(opera::evt::HbbTVBroadcastRelatedStatusChanged);
    ctrl->AddEventHandler(&BrowserModeHBBTV::OnHbbTVKeysetChanged, this); //(opera::evt::HbbTVKeysetChanged);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnIMECancelled,this);//(opera::evt::IMECancelled);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnIMEStart,this);//(opera::evt::IMEStart);
    ctrl->AddEventHandler(&BrowserModeHBBTV::OnLoadingBlocked, this); //(opera::evt::LoadingBlocked);
    ctrl->AddEventHandler(&BrowserModeHBBTV::OnLoadingFinished, this); //(opera::evt::LoadingFinished);
    ctrl->AddEventHandler(&BrowserModeHBBTV::OnLoadingStarted, this); //(opera::evt::LoadingStarted);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnMimeTypeRetrieved,this);//(opera::evt::MimeTypeRetrieved);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnNewWindowBlocked,this);//(opera::evt::NewWindowBlocked);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnNewWindow,this);//(opera::evt::NewWindow);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnOIPFApplicationActivated,this);//(opera::evt::OIPFApplicationActivated);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnOIPFApplicationDeactivated,this);//(opera::evt::OIPFApplicationDeactivated);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnOIPFApplicationStarted,this);//(opera::evt::OIPFApplicationStarted);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnOIPFApplicationStartFailed,this);//(opera::evt::OIPFApplicationStartFailed);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnOIPFApplicationStopped,this);//(opera::evt::OIPFApplicationStopped);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnPlatformMessage,this);//(opera::evt::PlatformMessage);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnRemoteXmlAITDownloadStatus,this);//(opera::evt::RemoteXmlAITDownloadStatus);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnRendererProcessDied,this);//(opera::evt::RendererProcessDied);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnRenderingBufferUpdate,this);//(opera::evt::RenderingBufferUpdate);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnRequestQuotaPermission,this);//(opera::evt::RequestQuotaPermission);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnResetDialogState,this);//(opera::evt::ResetDialogState);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnRunBeforeUnloadDialog,this);//(opera::evt::RunBeforeUnloadDialog);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnRunJavaScriptDialog,this);//(opera::evt::RunJavaScriptDialog);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnScriptRequestCloseWindow,this);//(opera::evt::ScriptRequestCloseWindow);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnScriptRequestWindowChangeBounds,this);//(opera::evt::ScriptRequestWindowChangeBounds);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnTVStoreWindowClosed,this);//(opera::evt::TVStoreWindowClosed);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnUnhandledKeyInput,this);//(opera::evt::UnhandledKeyInput);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnURLChanged,this);//(opera::evt::URLChanged);
    // ctrl->AddEventHandler(&BrowserModeHBBTV::OnVisibleSSLStateChanged,this);//(opera::evt::VisibleSSLStateChanged);

}

void BrowserModeHBBTV::OnAITParsingStatus(opera::evt::AITParsingStatus evt)
{
    TRACE_HERE();
    if (!m_HbbTV) return;
    m_HbbTV->OnAITParsingStatus(evt);
}

void BrowserModeHBBTV::OnAITUpdated(opera::evt::AITUpdated evt)
{
    TRACE_HERE();
    if (!m_HbbTV) return;
    m_HbbTV->OnAITUpdated(evt);
}

void BrowserModeHBBTV::OnCertificateError(opera::evt::CertificateError evt)
{

    opera::Handle window_id = evt.window_id;

    opera::evt::CertificateError::SSLInfo ssl_info = evt.ssl_info;
    BLOGW("Certificate Error: %s |for w_id:%s", window_id.str().c_str(), evt.ToString(ssl_info.certificate_error).c_str());

}

void BrowserModeHBBTV::OnHbbTVApplicationBlocked(opera::evt::HbbTVApplicationBlocked evt)
{
    if (!m_HbbTV) return;
    m_HbbTV->OnHbbTVApplicationBlocked(evt);
}

void BrowserModeHBBTV::OnHbbTVApplicationLoadTimedOut(opera::evt::HbbTVApplicationLoadTimedOut evt)
{
    if (!m_HbbTV) return;
    m_HbbTV->OnHbbTVApplicationLoadTimedOut(evt);
}

void BrowserModeHBBTV::OnHbbTVApplicationNotStarted(opera::evt::HbbTVApplicationNotStarted evt)
{
    if (!m_HbbTV) return;
    m_HbbTV->OnHbbTVApplicationNotStarted(evt);

    if (!hbbtv_controller_ptr_) {
        BLOGW("hbbtv controller not started yet, or something bad happened\n");
        return;
    }

    hbbtv_controller_ptr_->HbbTVApplicationNotStarted(evt);
}

void BrowserModeHBBTV::OnHbbTVApplicationStarted(opera::evt::HbbTVApplicationStarted evt)
{
    if (!m_HbbTV) return;
    m_HbbTV->OnHbbTVApplicationStarted(evt);

    BLOGV("Started App:window:(%s),atb:(%d,%d,%d),ids:(%d,%d),url: %s",
          evt.window_id.str().c_str(),
          evt.autostart_launching,
          evt.is_trusted,
          evt.broadcast_related,
          evt.app_id,
          evt.org_id,
          evt.app_url.c_str());

    opera::Handle window_id = getCurrentHbbTVWindow();

    if (evt.window_id != window_id) {
        BLOGW("Ignore events from othe window. ");
        return;
    }
    if (!hbbtv_controller_ptr_) {
        BLOGW("hbbtv controller not started yet, or something bad happened\n");
        return;
    }
    hbbtv_controller_ptr_->HbbTVApplicationStarted(evt);

    if (evt.broadcast_related) {
        BLOGI("Broadcast related application has started\n");
    } else {
        BLOGI("Broadcast independent application with URL: [%s] has started\n",
              evt.app_url.c_str());
        updateKeySet(opera::msg::CreateHbbTVWindow::HBBTV_KEY_ALL);
    }
}

void BrowserModeHBBTV::OnHbbTVApplicationStopped(opera::evt::HbbTVApplicationStopped evt)
{
    if (!m_HbbTV) return;
    m_HbbTV->OnHbbTVApplicationStopped(evt);

    if (!hbbtv_controller_ptr_) {
        BLOGW("hbbtv controller not started yet, or something bad happened\n");
        return;
    }
    unsigned int current_app_id, current_app_orgid;
    hbbtv_controller_ptr_->getCurrentAppID(current_app_id, current_app_orgid);
    if (current_app_id == evt.app_id && current_app_orgid == evt.org_id) {
        updateKeySet(0);
    }
    requestResumeDTVAsync();
    setTVVideoWindowToFullScreenIfNeeded();
    base::set_process_property(PROPERTY_BROWSER_HBBTV_APPLICATION_STOPED, "true");

}

void BrowserModeHBBTV::requestResumeDTVAsync()
{

    if (m_HbbTV && m_HbbTV->isPlayingDTV()) {
        BLOGA("DTV is playing, Just restore window to full screen");
        setTVVideoWindowToFullScreenIfNeeded();
    } else {
        BLOGA("Sending Resume DTV Request");
        base::set_process_property(PROPERTY_BROWSER_HAS_RESUME_BROADCAST_REQUEST, "true");
    }
}

void BrowserModeHBBTV::OnHbbTVApplicationVisibilityChanged(opera::evt::HbbTVApplicationVisibilityChanged evt)
{
    if (!m_HbbTV) {
        return;
    }

    m_HbbTV->OnHbbTVApplicationVisibilityChanged(evt);
    if (!hbbtv_controller_ptr_) {
        BLOGW("hbbtv controller not started yet, or something bad happened\n");
        return;
    }
    hbbtv_controller_ptr_->OnHbbTVApplicationVisibilityChanged(evt);
    //Incase the application is just stopped and be visiable again:
    if (evt.visible) {
        BLOGI("App(%d,%d) Shown.", evt.org_id, evt.app_id);
        base::set_process_property(PROPERTY_BROWSER_HBBTV_APPLICATION_VISIABLE, "true");
        BrowserInput & input = browser().getBrowserInput();
        int currentKeyset = input.getCurrentKeyset();
        int appKeyset = m_HbbTV->getAppLatestKeySet(evt.org_id, evt.app_id, evt.app_url);


        if (currentKeyset == 0 && appKeyset > 0) {
            BLOGW("Restore keyset for App(%d,%d), %d(0x%03X) -> %d(0x%03X)",
                  evt.org_id, evt.app_id,
                  currentKeyset, currentKeyset,
                  appKeyset, appKeyset);
            updateKeySet(appKeyset);
        } else {
            BLOGI("Current Keyset: %d(%03X)", currentKeyset);
        }
    } else {
        base::set_process_property(PROPERTY_BROWSER_HBBTV_APPLICATION_VISIABLE, "false");
        BLOGI("App(%d,%d) Hidden.", evt.org_id, evt.app_id);
    }
    updateMediaState();
    bool autostart = m_HbbTV->isAutoStartApplication(evt.org_id, evt.app_id, evt.app_url);
    if (autostart) {
        base::set_process_property(PROPERTY_BROWSER_HBBTV_APPLICATION_AUTOSTART, "true");
    } else {
        base::set_process_property(PROPERTY_BROWSER_HBBTV_APPLICATION_AUTOSTART, "false");
    }
    browser().getConfiguration().hbbtvOSDStatusChange();
}

void BrowserModeHBBTV::OnHbbTVBroadcastRelatedStatusChanged(opera::evt::HbbTVBroadcastRelatedStatusChanged evt)
{
    if (!m_HbbTV) return;
    m_HbbTV->OnHbbTVBroadcastRelatedStatusChanged(evt);
}

void BrowserModeHBBTV::OnHbbTVKeysetChanged(opera::evt::HbbTVKeysetChanged evt)
{
    if (!m_HbbTV) return;
    m_HbbTV->OnHbbTVKeysetChanged(evt);

    BLOGI("called, app_id: %d, org_id: %d, app_url: %s, broadcast_related: %d, "
          "new_keyset: 0x%x\n",
          evt.app_id, evt.org_id, evt.app_url.c_str(),
          evt.broadcast_related, evt.new_keyset);
    updateKeySet(evt.new_keyset);
    updateMediaState();
}

void BrowserModeHBBTV::OnLoadingBlocked(opera::evt::LoadingBlocked evt)
{
    BLOGS("Loading Blocked: w_id:%s,url:%s",
          evt.window_id.str().c_str(),
          evt.url.c_str());
    updateMediaState();
    NotImplemented();
}

void BrowserModeHBBTV::OnLoadingFinished(opera::evt::LoadingFinished evt)
{
    //    if(true == hbbtv_controller_ptr_->broadcast_integration_.GetHbbtvTeletextProcess())
    //    {
    //        hbbtv_controller_ptr_->broadcast_integration_.SetHbbtvTeletextStatus(BroadcastIntegration::TELETEXT_STARTED);
    //    }

    opera::Handle window_id = evt.window_id;

    std::string url = evt.url;

    opera::evt::LoadingFinished::Status status = evt.status;

    int error_code = evt.error_code;

    std::string error_message = evt.error_message;

    int http_status_code = evt.http_status_code;

    BLOGS("Loading Finished: win_id:%s,status:%s,err:%d(%s),http:%d,url:%s",
          window_id.str().c_str(),
          evt.ToString(status).c_str(),
          error_code, error_message.c_str(),
          http_status_code,
          url.c_str());

    //FIXME: Need Red key?
    // updateKeySet(0x01);
    updateMediaState();
    NotImplemented();
}

void BrowserModeHBBTV::OnLoadingStarted(opera::evt::LoadingStarted evt)
{
    //    if(true == hbbtv_controller_ptr_->broadcast_integration_.GetHbbtvTeletextProcess())
    //    {
    //        hbbtv_controller_ptr_->broadcast_integration_.SetHbbtvTeletextStatus(BroadcastIntegration::TELETEXT_STARTING);
    //    }
    BLOGS("HBBTV Loading Started: %s", evt.url.c_str());
    NotImplemented();
    updateMediaState();
}

//This is a Cross Thread Function:

void BrowserModeHBBTV::onDTVStatusChanged(bool status)
{
    BLOGV("DTV/VOD statue changed : %d", status);
    updateMediaState();
}

void BrowserModeHBBTV::updateMediaState()
{
    if (!m_HbbTV) {
        BLOGW("No HbbTV Integraion.");
        return;
    }


    bool changed = m_HbbTV->updateMediaState();

    if (changed) {
        bool showingVOD = m_HbbTV->isPlayingVOD();

        bool subwindow = m_HbbTV->isVideoInSubWindowMode();

        std::string media_type = showingVOD ? "VOD" : "TV";
        std::string window_type = subwindow ? "Subwindow" : "Unknown";

        BLOGV("MediaState Changed::media:%s,window:%s ", media_type.c_str(), window_type.c_str());

        browser().mediaStateChanged(BROWSER_MODE_HBBTV, media_type, window_type);
    } else {
        // BLOGV("MediaState:media:%s,window:%s ", media_type.c_str(), window_type.c_str());
        BLOGV("MediaState is not changed.");
    }

}

void BrowserModeHBBTV::setTVVideoWindowToFullScreenIfNeeded()
{
    if (!m_HbbTV) {
        BLOGW("No HbbTV Integraion.");
        return;
    }
    if (m_HbbTV->isPlayingDTV() && m_HbbTV->isVideoInSubWindowMode()) {

        int setvideowindow = browser().getConfiguration().setTVToFullScreen();

        BLOGV("SetTVvideowindow to fullscreen!");
        if (setvideowindow != 0) {
            BLOGE("Failed to setvideowindow to fullscreen!");
        }
    }
}

