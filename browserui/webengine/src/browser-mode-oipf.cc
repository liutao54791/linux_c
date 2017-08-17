#include "browser-mode-oipf.h"
#include "browser-impl.h"
#include "browser_debug.h"
#include "browser-opera.h"
#include "browser-input.h"
#include "async_browser_task.h"
#include "task_manager.h"

#include <fstream>
#include <limits.h>
#include <algorithm>

#include <opera/messages/CreateHbbTVWindow.h>
#include <opera/messages/KeyInput.h>
#include <uva/utils/log.h>
#include "graphics.h"
#include "configuration.h"
#include "virtual-keyboard.h"
#include "NotImplemented.h"
#include "systeminfo.h"

BrowserModeOIPF::InputInterceptor::InputInterceptor(BrowserImpl* browserImpl)
: m_browserImpl(browserImpl)
{
}

bool BrowserModeOIPF::InputInterceptor::WouldConsume(
        const opera::msg::KeyInput& key_input)
{
    // TODO(sunkai5): only consume those in keyset
    switch (key_input.virtual_key)
    {
    case opera::msg::KeyInput::OMI_KEY_POWER:
    {
        if (key_input.event_type == opera::msg::KeyInput::KEYDOWN) {
            browser().setUrl(BROWSER_MODE_OFF, "");
        }
        return false;
    }
    default:
    {
        return true;
    }
    }
}

bool BrowserModeOIPF::InputInterceptor::InterceptKeyEvent(
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

BrowserModeOIPF::BrowserModeOIPF(BrowserImpl* browserImpl)
: BrowserMode(browserImpl)
{
    BROWSER_LOG(INFO, "");
}

BrowserModeOIPF::~BrowserModeOIPF()
{
    BROWSER_LOG(INFO, "");
}

BrowserStatus BrowserModeOIPF::start()
{
    BROWSER_LOG(INFO, "");

    m_input_interceptor = new InputInterceptor(&browser());
    if (!m_input_interceptor) {
        BROWSER_LOG(ERROR, "Failed to create input interceptor");
        return BrowserError;
    }

    return BrowserOK;
}

void BrowserModeOIPF::stop()
{
    BROWSER_LOG(INFO, "");

    HisenseOpera& opera = browser().getHisenseOpera();
    if (hbbtv_window_.IsValid())
        opera.destroyWindow(hbbtv_window_);

    if (m_input_interceptor) {
        delete m_input_interceptor;
        m_input_interceptor = NULL;
    }
}

void BrowserModeOIPF::enter(MultiModeArgument *argument)
{
    BLOGW("OIPF Enter, actually the HBBTV under AllApps.");
    Graphics& graphics = browser().getGraphics();
    graphics.requestFocus(false, true);

    BrowserInput& browserInput = browser().getBrowserInput();
    // Disable physical mouse input and enable key input
    browserInput.enableKeyInput(true);
    browserInput.enableMouseInput(false);

    HisenseOpera& opera = browser().getHisenseOpera();

    std::vector<opera::msg::CreateHbbTVWindow::PlatformProfile> platform_profiles{opera::msg::CreateHbbTVWindow::FREEVIEW_2_0};

    hbbtv_window_ = CreateHbbTVWindow(platform_profiles);

    opera.show(hbbtv_window_);
    opera.focus(hbbtv_window_);

    opera.setPref(hbbtv_window_, "allow_scripts_to_close_windows", "true");
    opera.setPref(hbbtv_window_, "notify_scripts_closing_windows", "true");

    // HBBTV window don't support create HTML view
    // enter virtual keyboard
    // browser().getVirtualKeyboard().enter( m_main_window );

    // Set input interceptor
    if (m_input_interceptor)
        browserInput.setEventsIntercept(m_input_interceptor);

    opera.AddEventHandler(&BrowserModeOIPF::loadingStarted, this);
    opera.AddEventHandler(&BrowserModeOIPF::loadingFinished, this);
    //opera.AddEventHandler( &BrowserModeOIPF::ApplicationStarted, this );
    //opera.AddEventHandler( &BrowserModeOIPF::ApplicationStopped, this );//maybe used later.
    opera.AddEventHandler(&BrowserModeOIPF::OnHbbTVKeysetChanged, this); //(opera::evt::HbbTVKeysetChanged);
    opera.AddEventHandler(&BrowserModeOIPF::OnHbbTVApplicationStopped, this);
    opera.AddEventHandler(&BrowserModeOIPF::ScriptRequestCloseWindow, this);

    opera.resume();
}

void BrowserModeOIPF::leave()
{
    BROWSER_LOG(INFO, "Leave");

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

    opera.terminateApplication(hbbtv_window_, true);
    opera.hide(hbbtv_window_);
    opera.destroyWindow(hbbtv_window_);
    // Set input interceptor to NULL
    browserInput.setEventsIntercept(NULL);
    Graphics& graphics = browser().getGraphics();
    graphics.lostFocus();
}

void BrowserModeOIPF::setUrl(std::string url)
{
    BROWSER_LOG(INFO, "");
    HisenseOpera& opera = browser().getHisenseOpera();

    opera.loadBroadcastIndependentApplication(hbbtv_window_, url);
}

opera::Handle BrowserModeOIPF::CreateHbbTVWindow(
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

void BrowserModeOIPF::ScriptRequestCloseWindow(opera::evt::ScriptRequestCloseWindow event)
{
    BROWSER_LOG(INFO, "Got ScriptRequestCloseWindow window_id %s opened_by_dom %d url %s",
                event.id.str().c_str(), event.opened_by_dom, event.url.c_str());
    browser().getTaskManager().postTask(hisense::AsyncBrowserTask::create(&browser(), "pause"));
}

void BrowserModeOIPF::OnHbbTVKeysetChanged(opera::evt::HbbTVKeysetChanged evt)
{

    BLOGI("new Keyset: %d(0x%X)", evt.new_keyset, evt.new_keyset);
    BrowserInput& browserInput = browser().getBrowserInput();
    browserInput.updateKeySet(BROWSER_MODE_OIPF, evt.new_keyset);
}

void BrowserModeOIPF::loadingStarted(opera::evt::LoadingStarted event)
{
    BROWSER_LOG(ALWAYS, "Loading started window: %s %s", event.window_id.str().c_str(), event.url.c_str());
}

void BrowserModeOIPF::loadingFinished(opera::evt::LoadingFinished event)
{
    std::string status_str;
    switch (event.status)
    {
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

    BROWSER_LOG(ALWAYS, "Loading finished (%s) window: %s %s", status_str.c_str(), event.window_id.str().c_str(), event.url.c_str());
}

void BrowserModeOIPF::OnHbbTVApplicationStopped(opera::evt::HbbTVApplicationStopped event)
{

    browser().getTaskManager().postTask(hisense::AsyncBrowserTask::create(&browser(), "pause"));
}

