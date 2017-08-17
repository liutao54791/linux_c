#include <opera/Opera.h>
#include <opera/components/opera_controller.h>
#include <opera/components/integration_utils.h>
#include <opera/components/input/linux_input.h>
//#include <opera/browser_mode.h>

#include "browser-impl.h"
#include "browser-mode.h"
#include "browser-mode-off.h"
#include "browser-mode-store.h"
#include "browser-mode-opera-store.h"
#include "browser-mode-hbbtv.h"
#include "browser-mode-youtube.h"
#include "browser-mode-oipf.h"
#include "browser-mode-hibrowser.h"
#include "configuration.h"
#include "browser-input.h"
#include "graphics.h"
#include "browser-opera.h"
#include "virtual-keyboard.h"
#include "browser-uva-backends.h"

#include "signal_proc.h"
#include "browser_debug.h"
#include "language.h"
#include "browser-storage.h"
#include "browser_mouse.h"
#include "sqlite_mgr.h"
#include "socket-manager.h"
#include "memory-controller.h"
#include "NotImplemented.h"
#include "task_manager.h"

void setBrowserMode(BrowserModeType mode)
{
    NotImplemented();
}

BrowserImpl::BrowserImpl(int argc, char* argv[])
: m_currentMode(BROWSER_MODE_OFF)
, m_defaultMode(BROWSER_MODE_OFF)
, m_browserModes()
, m_argc(argc)
, m_argv(argv)
, m_running(false)
, m_inStealth(false)
{
    BROWSER_LOG(INFO, "");
    ///TODO:debug
}

BrowserImpl::~BrowserImpl()
{
    BROWSER_LOG(INFO, "");
}

void BrowserImpl::setEventListener(BrowserEventListener *listener)
{
    m_listener.reset(listener);
}

void BrowserImpl::keySetChanged(BrowserModeType mode, unsigned keyset)
{
    if (m_listener) {
        m_listener->OnKeySetChanged(mode, keyset);
    }
}

void BrowserImpl::appStarted(BrowserModeType mode, const std::string &appurl)
{
    if (m_listener) {
        m_listener->OnAppStarted(mode, appurl);
    }
}

void BrowserImpl::appStopped(BrowserModeType mode, const std::string &appurl)
{
    if (m_listener) {
        m_listener->OnAppStopped(mode, appurl);
    }
}

BrowserStatus BrowserImpl::start()
{
    createComponents();
    if (startComponents() != BrowserOK) {
        BROWSER_LOG(ERROR, "Failed to start all components");
        return BrowserError;
    }

    if (!m_running)
        m_running = true;

    m_browserModes.at(m_currentMode)->enter();

    // Enter OFF mode
    if (m_currentMode != m_defaultMode) {
        m_browserModes.at(m_currentMode)->leave();
        m_currentMode = m_defaultMode;
        m_browserModes.at(m_defaultMode)->enter();
    }
    setBrowserMode(m_currentMode);

    return BrowserOK;
}

BrowserStatus BrowserImpl::stop()
{
    if (!m_running) {
        BROWSER_LOG(WARNING, "Browser is already stoped");
        return BrowserError;
    }

    setUrl(BROWSER_MODE_OFF, "");
    if (stopComponents() != BrowserOK) {
        BROWSER_LOG(ERROR, "Failed to stop all components");
        return BrowserError;
    }

    if (m_running)
        m_running = false;

    return BrowserOK;
}

void BrowserImpl::pause()
{
    BrowserModeType mode = currentMode();
    if (mode == BROWSER_MODE_HBBTV) {
        BLOGI("Pause BROWSER_MODE_HBBTV");
        m_browserModes[ BROWSER_MODE_HBBTV ]->pause();
    } else {
        BLOGI("Pause BrowserModeType:%d", mode);
        setUrl(BROWSER_MODE_OFF, "");
    }

}

void BrowserImpl::resume()
{
    BrowserModeType mode = currentMode();
    if (mode == BROWSER_MODE_HBBTV) {
        BLOGI("Resume BROWSER_MODE_HBBTV");
        m_browserModes[ BROWSER_MODE_HBBTV ]->pause();
    } else {
        BLOGW("Don't know how to resume BrowserModeType= %d.", mode);
    }
}

void BrowserImpl::pauseHbbTV()
{
    BROWSER_LOG(INFO, "called");
    m_browserModes[ BROWSER_MODE_HBBTV ]->pause();
}

void BrowserImpl::resumeHbbTV()
{
    BROWSER_LOG(INFO, "called");
    m_browserModes[ BROWSER_MODE_HBBTV ]->resume();
}
bool BrowserImpl::isInStealthMode()
{
    //Only HBBTV can enter stealth mode:
    if(m_currentMode != BROWSER_MODE_HBBTV){
        return false;
    }

   return m_inStealth;

}

void BrowserImpl::updateStealthStatus(bool stealth)
{
    if (m_inStealth != stealth) {
        BLOGW("%s Stealth Mode.", stealth ? "Enter" : "Leave");
        m_inStealth = stealth;
    }
}
void BrowserImpl::createComponents()
{
    m_browserSignal.reset(new BrowserSignal(this));

    m_browserStorage.reset(new BrowserStorage(this));
    m_browserLang.reset(new BrowserLanguage(this));
    m_configuration.reset(new Configuration(this, m_argc, m_argv));
    m_memroyController.reset(new MemroyController(this));
    m_graphics.reset(new Graphics(this));
    m_browserInput.reset(new BrowserInput(this));
    m_opera.reset(new HisenseOpera(this));
    m_virtualKeyboard.reset(new VirtualKeyboard(this));
    m_browserMouse.reset(new BrowserMouse(this));
    m_uvaBackend.reset(new BrowserUVABackend(this));
    m_sqliteManager.reset(new SqliteManager(this));
    m_socketManager.reset(new SocketManager(this));
    m_taskManager.reset(new TaskManager(this));

    m_browserModes[ BROWSER_MODE_OFF ] = std::shared_ptr<BrowserMode>(new BrowserModeOFF(this));
    //If HBBTV is disabled we use OFF mode to replace it and it will never be used:
#if ENABLE(HBBTV)
    m_browserModes[ BROWSER_MODE_HBBTV ] = std::shared_ptr<BrowserMode>(new BrowserModeHBBTV(this));
#else
    m_browserModes[ BROWSER_MODE_HBBTV ] = m_browserModes[ BROWSER_MODE_OFF ];
#endif

    m_browserModes[ BROWSER_MODE_STORE ] = std::shared_ptr<BrowserMode>(new BrowserModeStore(this));
    m_browserModes[ BROWSER_MODE_OPERA_STORE ] = std::shared_ptr<BrowserMode>(new BrowserModeOperaStore(this));
    m_browserModes[ BROWSER_MODE_OIPF ] = std::shared_ptr<BrowserMode>(new BrowserModeOIPF(this));
    m_browserModes[ BROWSER_MODE_HIBROWSER ] = std::shared_ptr<BrowserMode>(new BrowserModeHiBrowser(this));
    m_browserModes[ BROWSER_MODE_YOUTUBE ] = std::shared_ptr<BrowserMode>(new BrowserModeYouTube(this));
}

BrowserStatus BrowserImpl::startComponents()
{
    BrowserStatus status;

    status = m_browserSignal->start();
    if (status != BrowserOK) {
        return status;
    }
    BROWSER_LOG(ALWAYS, "Succeed to start Signal Handler");

    status = m_browserStorage->start();
    if (status != BrowserOK) {
        return status;
    }
    BROWSER_LOG(ALWAYS, "Succeed to start browser storage");

    status = m_browserLang->start();
    if (status != BrowserOK) {
        return status;
    }
    BROWSER_LOG(ALWAYS, "Succeed to start Language");

    status = m_configuration->start();
    if (status != BrowserOK) {
        return status;
    }
    BROWSER_LOG(ALWAYS, "Succeed to start Configuration");

    status = m_memroyController->start();
    if (status != BrowserOK) {
        return status;
    }
    BROWSER_LOG(ALWAYS, "Succeed to start MemoryController");

    status = m_graphics->start();
    if (status != BrowserOK) {
        return status;
    }
    BROWSER_LOG(ALWAYS, "Succeed to start Graphics");

    status = m_opera->start();
    if (status != BrowserOK) {
        return status;
    }
    BROWSER_LOG(ALWAYS, "Succeed to start HisenseOpera");

    status = m_browserInput->start();
    if (status != BrowserOK) {
        return status;
    }
    BROWSER_LOG(ALWAYS, "Succeed to start BrowserInput");

    status = m_virtualKeyboard->start();
    if (status != BrowserOK) {
        return status;
    }
    BROWSER_LOG(ALWAYS, "Succeed to start Virtual Keyboard");

    status = m_browserMouse->start();
    if (status != BrowserOK) {
        //Failed is OK.
        BLOGA("Failed to start BrowserMouse");
    } else {
        BLOGA("Succeed to start BrowserMouse");
    }

    status = m_uvaBackend->start();
    if (status != BrowserOK) {
        return status;
    }
    BROWSER_LOG(ALWAYS, "Succeed to start UVA backend");

    status = m_sqliteManager->start();
    if (status != BrowserOK) {
        return status;
    }
    BROWSER_LOG(ALWAYS, "Succeed to start Sqlite Manager");

    status = m_socketManager->start();
    if (status != BrowserOK) {
        return status;
    }
    BROWSER_LOG(ALWAYS, "Succeed to start Socket Manager");

    status = m_taskManager->start();
    if (status != BrowserOK) {
        return status;
    }
    BROWSER_LOG(ALWAYS, "Succeed to start Task Manager");

    status = m_browserModes[ BROWSER_MODE_OFF ]->start();
    if (status != BrowserOK) {
        return status;
    }

    status = m_browserModes[ BROWSER_MODE_STORE ]->start();
    if (status != BrowserOK) {
        return status;
    }

    status = m_browserModes[ BROWSER_MODE_OPERA_STORE ]->start();
    if (status != BrowserOK) {
        return status;
    }

    status = m_browserModes[ BROWSER_MODE_OIPF ]->start();
    if (status != BrowserOK) {
        return status;
    }

    status = m_browserModes[ BROWSER_MODE_HIBROWSER ]->start();
    if (status != BrowserOK) {
        return status;
    }


    status = m_browserModes[ BROWSER_MODE_HBBTV ]->start();
    if (status != BrowserOK) {
        return status;
    }


    status = m_browserModes[ BROWSER_MODE_YOUTUBE ]->start();
    if (status != BrowserOK) {
        return status;
    }
    BROWSER_LOG(ALWAYS, "Succeed to start all browser modes");

    return status;
}

BrowserStatus BrowserImpl::stopComponents()
{
    m_browserModes[ BROWSER_MODE_OPERA_STORE ]->stop();
    m_browserModes[ BROWSER_MODE_STORE ]->stop();

    m_browserModes[ BROWSER_MODE_HBBTV ]->stop();

    m_browserModes[ BROWSER_MODE_OIPF ]->stop();
    m_browserModes[ BROWSER_MODE_HIBROWSER ]->stop();
    m_browserModes[ BROWSER_MODE_YOUTUBE ]->stop();
    m_browserModes[ BROWSER_MODE_OFF ]->stop();

    m_socketManager->stop();
    m_sqliteManager->stop();
    m_uvaBackend->stop();
    m_browserMouse->stop();
    m_virtualKeyboard->stop();
    m_browserInput->stop();
    m_opera->stop();
    m_graphics->stop();
    m_memroyController->stop();
    m_configuration->stop();
    m_browserLang->stop();
    m_browserStorage->stop();
    m_browserSignal->stop();

    BROWSER_LOG(ALWAYS, "Stop all components");

    return BrowserOK;
}

void BrowserImpl::setUrl(BrowserModeType mode, std::string url, MultiModeArgument *argument)
{
    BROWSER_LOG(ALWAYS, "mode = %d, url = %s", mode, url.c_str());

    if ((mode < BROWSER_MODE_TYPE_MIN) || (mode > BROWSER_MODE_TYPE_MAX)) {
        BROWSER_LOG(ERROR, "Invalued browser mode type!");
        return;
    }

    //The UI might call resume HbbTV_AIT even when TTX/Mheg5 is running,
    //We need to block this request, and the HbbTV will be resumed when
    //exiting TTX/Mheg5
    if(url.empty() && mode == BROWSER_MODE_HBBTV && isInStealthMode()){
        BLOGE("Ignore UI request (Resuming HbbTV) when TTX/Mheg is running ");
        return ;
    }
    updateStealthStatus(false);
    if (mode != m_currentMode) {
        m_browserModes.at(m_currentMode)->leave();
        appStopped(m_currentMode, "");

        m_currentMode = mode;
        m_browserModes.at(m_currentMode)->enter(argument);

    }

    m_browserModes.at(m_currentMode)->setUrl(url);
    //The HBBTV mode is started only after the webapp is loaded
    //The STORE mode notify appstarted to APM after apploading circle display
    if (m_currentMode != BROWSER_MODE_HBBTV && m_currentMode != BROWSER_MODE_STORE) {
        appStarted(m_currentMode, url);
    }
    setBrowserMode(m_currentMode);
}

void BrowserImpl::injectXmlAit(std::string url)
{
    BROWSER_LOG(INFO, "url: %s", url.c_str());
    updateStealthStatus(false);
    if (BROWSER_MODE_HBBTV != m_currentMode) {
        m_browserModes.at(m_currentMode)->leave();
        m_currentMode = BROWSER_MODE_HBBTV;
        m_browserModes.at(m_currentMode)->enter();
    }

    m_browserModes.at(BROWSER_MODE_HBBTV)->injectXmlAit(url);
}

void BrowserImpl::setNetStatus(const int status)
{
    BROWSER_LOG(INFO, "status: %d", status);
    updateStealthStatus(false);
    if (BROWSER_MODE_HBBTV != m_currentMode) {

        m_browserModes.at(m_currentMode)->leave();
        m_currentMode = BROWSER_MODE_HBBTV;
        m_browserModes.at(m_currentMode)->enter();
    }

    m_browserModes.at(BROWSER_MODE_HBBTV)->setNetStatus(status);
}

void BrowserImpl::ReTune()
{
    BROWSER_LOG(INFO, "called!");
    updateStealthStatus(false);
    if (BROWSER_MODE_HBBTV != m_currentMode) {

        m_browserModes.at(m_currentMode)->leave();
        m_currentMode = BROWSER_MODE_HBBTV;
        m_browserModes.at(m_currentMode)->enter();
    }

    m_browserModes.at(BROWSER_MODE_HBBTV)->ReTune();
}

void BrowserImpl::processOOM()
{
    BROWSER_LOG(ALWAYS, "System OOM");
    m_browserModes.at(m_currentMode)->processOOM();
}

Configuration& BrowserImpl::getConfiguration()
{
    return *m_configuration;
}

BrowserInput& BrowserImpl::getBrowserInput()
{
    return *m_browserInput;
}

Graphics& BrowserImpl::getGraphics()
{
    return *m_graphics;
}

HisenseOpera& BrowserImpl::getHisenseOpera()
{
    return *m_opera;
}

VirtualKeyboard& BrowserImpl::getVirtualKeyboard()
{
    return *m_virtualKeyboard;
}

BrowserLanguage& BrowserImpl::getLanguage()
{
    return *m_browserLang;
}

BrowserMouse& BrowserImpl::getBrowserMouse()
{
    return *m_browserMouse;
}

SqliteManager& BrowserImpl::getSqliteManager()
{
    return *m_sqliteManager;
}

MemroyController& BrowserImpl::getMemoryController()
{
    return *m_memroyController;
}

TaskManager& BrowserImpl::getTaskManager()
{
    return *m_taskManager;
}

void BrowserImpl::appMessageArrived(BrowserModeType mode, const std::string & message)
{
    if (m_listener) {

        m_listener->OnAppMessage(mode, message);
    }
}

void BrowserImpl::notifyApp(BrowserModeType mode, const std::string & message)
{
    if (m_currentMode != mode) {
        BLOGW("mode(%d) is not current(%d), message(%s)", mode, m_currentMode, message.c_str());
        return;
    }
    m_browserModes.at(m_currentMode)->sendMessage(message);
}
   void BrowserImpl::mediaStateChanged(BrowserModeType mode,const std::string & media,const std::string &subwindow)
   {
       if (m_listener) {

           std::string msg ;
           msg += "{ \"type\":\"HbbTVMediaState\",\"MediaType\":\"";
           msg += media;
           msg += "\",\"SubwindowState\":\"";
           msg += subwindow;
           msg += "\"}";

           m_listener->OnAppMessage(mode, msg);
       }
   }
//void BrowserImpl::postTask(std::shared_ptr<hisense::AsyncBrowserTask>task)
//{
//    m_taskManager->postTask(task);
// }