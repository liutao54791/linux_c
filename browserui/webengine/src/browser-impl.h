#ifndef BROWSER_IMPL_H
#define BROWSER_IMPL_H

#include <map>
#include <memory>
#include <string>

#include "browser_type.h"
#include "async_browser_task.h"

class BrowserMode;
class BrowserModeOFF;
class BrowserModeStore;
class BrowserModeHBBTV;
class BrowserModeYouTube;
class Configuration;
class Graphics;
class HisenseOpera;
class BrowserInput;
class VirtualKeyboard;
class BrowserUVABackend;

class BrowserSignal;
class BrowserLanguage;
class BrowserStorage;
class BrowserMouse;
class SqliteManager;
class SocketManager;
class MemroyController;
class TaskManager;

class BrowserEventListener
{
public:

    virtual ~BrowserEventListener() { }
    virtual void OnKeySetChanged(BrowserModeType appmode, unsigned int) = 0;
    virtual void OnAppStarted(BrowserModeType appmode, const std::string & appname) = 0;
    virtual void OnAppStopped(BrowserModeType appmode, const std::string & appname) = 0;
    virtual void OnAppMessage(BrowserModeType appmode, const std::string & message) = 0;

};

class BrowserImpl
{
public:
    BrowserImpl(int argc, char* argv[]);
    ~BrowserImpl();

    BrowserStatus start();
    BrowserStatus stop();
    void pause();
    void resume();


    void pauseHbbTV();
    void resumeHbbTV();
    void injectXmlAit(std::string url);
    void setNetStatus(const int status);
    void ReTune();
    bool isInStealthMode();
    void updateStealthStatus(bool);
    void setEventListener(BrowserEventListener *);
    void keySetChanged(BrowserModeType mode, unsigned keyset);
    void appStarted(BrowserModeType mode, const std::string &appurl);
    void appStopped(BrowserModeType mode, const std::string &appurl);
    void mediaStateChanged(BrowserModeType mode,const std::string & source,const std::string &subwindow);
    void appMessageArrived(BrowserModeType mode, const std::string & message);
    void notifyApp(BrowserModeType mode, const std::string & message);
    void setUrl(BrowserModeType mode, std::string url = "", MultiModeArgument *argument = NULL);
    //  void postTask(std::shared_ptr<hisense::AsyncBrowserTask>task);
    Configuration& getConfiguration();
    BrowserInput& getBrowserInput();
    Graphics& getGraphics();
    HisenseOpera& getHisenseOpera();
    VirtualKeyboard& getVirtualKeyboard();
    BrowserLanguage& getLanguage();
    BrowserMouse& getBrowserMouse();
    SqliteManager& getSqliteManager();
    MemroyController& getMemoryController();
    TaskManager& getTaskManager();

    bool isRunning()
    {
        return m_running;
    }

    void processOOM();

    BrowserModeType currentMode() const
    {
        return m_currentMode;
    }

private:
    void createComponents();
    BrowserStatus startComponents();
    BrowserStatus stopComponents();

private:
    BrowserModeType m_currentMode;
    BrowserModeType m_defaultMode;
    std::map< BrowserModeType, std::shared_ptr<BrowserMode> > m_browserModes;

    std::shared_ptr<BrowserSignal> m_browserSignal;
    std::shared_ptr<BrowserStorage> m_browserStorage;
    std::shared_ptr<BrowserLanguage> m_browserLang;
    std::shared_ptr<Configuration> m_configuration;
    std::shared_ptr<MemroyController> m_memroyController;
    std::shared_ptr<Graphics> m_graphics;
    std::shared_ptr<BrowserInput> m_browserInput;
    std::shared_ptr<HisenseOpera> m_opera;
    std::shared_ptr<VirtualKeyboard> m_virtualKeyboard;
    std::shared_ptr<BrowserMouse> m_browserMouse;
    std::shared_ptr<BrowserUVABackend> m_uvaBackend;
    std::shared_ptr<SqliteManager> m_sqliteManager;
    std::shared_ptr<SocketManager> m_socketManager;
    std::shared_ptr<TaskManager> m_taskManager;
    std::shared_ptr<BrowserEventListener> m_listener;
    int m_argc;
    char** m_argv;

    bool m_running;
    bool m_inStealth;

};

#endif
