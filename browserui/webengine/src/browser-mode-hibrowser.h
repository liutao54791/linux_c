#ifndef BROWSER_MODE_HIBROWSER_H
#define BROWSER_MODE_HIBROWSER_H

#include <opera/components/timer_listener.h>

#include <opera/events/BrowserMemoryEvent.h>
#include <opera/events/TitleWasSet.h>
#include <opera/events/FaviconWasSet.h>
#include <opera/events/LoadingFinished.h>
#include <opera/events/LoadingProgress.h>
#include <opera/events/LoadingStarted.h>
#include <opera/events/NewWindow.h>
#include <opera/events/NewWindowBlocked.h>
#include <opera/events/PlatformMessage.h>
#include <opera/events/RendererProcessDied.h>
#include <opera/events/URLChanged.h>

#include <opera/messages/KeyInput.h>
#include <opera/messages/MouseInput.h>

#include "browser-mode.h"
#include "omi_components/async_input_dispatcher_impl.h"
#if 0 //!defined DATABASE_IMPLEMENT
#include "bookmarkmanager.h"
#include "historymanager.h"
#endif
#include <vector>

#define MAX_NUM_TABS 6

#if 1 //def DATABASE_IMPLEMENT
class HistoryDBManager;
class BookmarkDBManager;
#endif

typedef void (*SET_THUMBNAIL_WINDOW_ID)(int);

class BrowserModeHiBrowser : public BrowserMode
{
public:
    BrowserModeHiBrowser(BrowserImpl* browserImpl);

    virtual ~BrowserModeHiBrowser();

    virtual BrowserStatus start();
    virtual void stop();
    virtual void enter(MultiModeArgument *argument);
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
            virtual bool getUiWindow(opera::Handle* ui_id)
            {
                return false;
            }

            virtual ~ActiveWindowGetter() { }
        };
        ExampleInputInterceptor(BrowserImpl* browserImpl);

        virtual bool InterceptKeyEvent(const opera::msg::KeyInput& key_input) OPERA_OVERRIDE;
        virtual bool InterceptMouseEvent(const opera::msg::MouseInput& mouse_input) OPERA_OVERRIDE;
        /**
         * Deletes the old one.
         * */
        void handleKeyForVirtualMouse(const opera::msg::KeyInput& Key,opera::Handle window_id);
        void setActiveWindowGetter(ActiveWindowGetter* active_window_getter);

        void setPlatformEventReceiver(opera::Handle receiver)
        {
            m_receiver = receiver;
        }

        void setVideoFullscreen(bool fullscreen)
        {
            m_videoFullscreen = fullscreen;
        }
        bool existsHardwareCursor();
    private:
        //using function to obtain the browser instance
        //so we can remove the raw pointer in the future:

        BrowserImpl &browser() const
        {
            return *m_browserImpl;
        }
        BrowserImpl* m_browserImpl;
        long last_tv_time;
        bool key_repeat;
        bool m_videoFullscreen;
        // When all toolchains will support C++11, this should be
        // changed to unique_ptr.
        std::auto_ptr<ActiveWindowGetter> active_window_getter_;

        opera::Handle m_receiver;
        opera::Handle m_UIWindow;
        CTimerListener mouseEvent_timeout;
        bool m_hardware_cursor_has_event;
        void scrollUp(const opera::Handle& window, int multiplication);
        void scrollDown(const opera::Handle& window, int multiplication);
    };

private:
    //opera::Handle m_status_window;
    std::string m_main_url;

    bool m_firstEntered;
    opera::Handle m_thumbnail_window;
    opera::Handle m_ui_view;
    std::string m_uiUrl;
    std::string m_U_url;
    std::string m_thumbnail_url;
    int m_thumbnail_width;
    int m_thumbnail_height;
    CTimerListener hideCursor_timeout;
    static bool mouseShown;
    ExampleInputInterceptor* m_input_interceptor;
    void onTitleWasSet(opera::evt::TitleWasSet event);
    void onFaviconWasSet(opera::evt::FaviconWasSet event);
    void loadingStarted(opera::evt::LoadingStarted event);
    void loadingFinished(opera::evt::LoadingFinished event);
    void onUrlChange(opera::evt::URLChanged event);
    void onPlatformMessage(opera::evt::PlatformMessage event);
    void onNewWindow(opera::evt::NewWindow event);
    void onNewWindowBlocked(opera::evt::NewWindowBlocked event);
    void HandleRendererProcessDied(opera::evt::RendererProcessDied process_died);
    void BrowserMemoryEvent(opera::evt::BrowserMemoryEvent event);
    void onLoadingProgress( opera::evt::LoadingProgress event);
    void setMouseStatus(bool shown);
    bool getMouseStatus();
    void createThumbnailWindow();

    void reachLimitRangeHandle(int x, int y) { }

    struct TapsWondow{
      opera::Handle m_main_window;
      std::string url;
      std::string title;
    };
    int tab_index;
    int activeTabIndex;
    int beforeTabIndex;
    std::vector<TapsWondow> main_window_vector;

#if 0 //!defined DATABASE_IMPLEMENT
    std::string getFileData(std::string fileName);
    void saveToFile(std::string data, std::string fileName);

    void initBookmark();
    void saveBookmark();
    void initHistory();
    void saveHistory();

    void initSpeeddial();
    void saveSpeeddial();
#endif

    void initCurrentWindowData();

    enum Type
    {
        CMD_ID_INVALID = 0,
        CMD_ID_AUTHENTICATIONRECEIVED,
        CMD_ID_ADDTAB,
        CMD_ID_REMOVETAB,
        CMD_ID_ACTIVATETAB,
        CMD_ID_LOADURL,
        CMD_ID_SCROLL,
        CMD_ID_MOUSECLICK,
        CMD_ID_MOUSEMOVE,
        CMD_ID_MOVECURSOR,
        CMD_ID_GETSCROLL, //"getScroll" 10
        CMD_ID_SHOWMOUSECURSOR,
        CMD_ID_HIDEMOUSECURSOR,
        CMD_ID_ZOOM,
        CMD_ID_GETZOOMFACTOR, //"getZoomFactor"
        CMD_ID_HISTORYMOVE,
        CMD_ID_RELOAD,
        CMD_ID_STOPLOAD,
        CMD_ID_FOCUS,
        CMD_ID_BLUR,
        CMD_ID_SHOW, //20
        CMD_ID_GETWINDOWSIZE, //"getWindowSize"
        CMD_ID_GETCURSORPOSITION, //"getCursorPosition"
        CMD_ID_GETTABTITLE, //"getTabTitle"
        CMD_ID_GETCERTIFICATEINFO, //"getCertificateInfo"
        CMD_ID_ISVIRTUALKEYBOARDVISIBLE, //"isvitualKeyboardVisible"
        CMD_ID_CREATETHUMBNAIL, //"createThumbnail"
        CMD_ID_CREATETHUMBNAILFORURL, //"createThumbnailForUrl"
        CMD_ID_SAVEBOOKMARK, //"saveBookmark"
        CMD_ID_MOVEBOOKMARK, //"moveBookmark"
        CMD_ID_DELETEBOOKMARK, //"deleteBookmark" 30
        CMD_ID_GETALLBOOKMARKS, //"getAllBookmarks"
        CMD_ID_GETHISTORY, //"getHistory"
        CMD_ID_CLEARHISTORY, //"clearHistory"
        CMD_ID_GETSPEEDDIALS, //"getSpeedDials"
        CMD_ID_ADDSPEEDDIAL, //"addSpeedDial"
        CMD_ID_MOVESPEEDDIAL, //"moveSpeedDial"
        CMD_ID_REMOVESPEEDDIAL, //"removeSpeedDial"
        CMD_ID_DIALOGCLOSED,
        CMD_ID_GETSETTINGS, //"getSettings"
        CMD_ID_GETHIGHLIGHTEDURL, //"getHighlightedUrl" 40
        CMD_ID_CHANGEPREFERENCES,
        CMD_ID_ISFEATUREENABLED, //"isFeatureEnabled"

        /*added*/
        CMD_ID_CAPTURE_THUMBNAIL, /*43*/
        CMD_ID_THUMBNAIL_DATA, /*44*/
        CMD_ID_WINDOWRESIZETO, //45
        CMD_ID_UPDATE_TITLE, //46

        CMD_ID_DELETEHISTORY, //"deleteHistory" 47
        CMD_ID_CLEARCOOKIES, //"clearCookies"48
        CMD_ID_UPDATE_ICON, //"updateIcon"49
        CMD_ID_FOCUS_MAIN_WINDOW, //"focusMainWindow"50
        CMD_ID_GET_CURSOR_AND_SCROLL, //51
        CMD_ID_VIDEO_FULLSCREEN, //52
        CMD_ID_VIDEO_PLAYED, //53
        CMD_ID_MODIFY_SPEEDDIAL,
        CMD_ID_VIDEO_BACKWARD, //55
        CMD_ID_VIDEOPLAYPAUSE, //56
        CMD_ID_VIDEOSTOP, //57
        CMD_ID_VIDEOFORWARD, //58
        CMD_ID_VIDEO_ENDED, //59
        CMD_ID_VIDEO_TIME_UPDATE, //60
        CMD_ID_VIDEO_WAITING, //61
        CMD_ID_VIDEO_PLAYING, //62
        CMD_ID_VIDEO_PAUSED, //63
        CMD_ID_VIDEO_SET_PERCENTAGE, //64
        CMD_ID_GETSPEEDDIALBYURL,
        CMD_ID_GET_VIRTUAL_KEYBOARD_STATUS,
        CMD_ID_DESTROY_THUMBNAIL_WINDOW,
        CMD_ID_VIDEO_EXIT_FULLSCREEN,
        CMD_ID_END
    };

#define MAC2STR(X) (char*)#X
    char* cmdString[CMD_ID_END] = {
                                   MAC2STR(CMD_ID_INVALID),
                                   MAC2STR(CMD_ID_AUTHENTICATIONRECEIVED),
                                   MAC2STR(CMD_ID_ADDTAB),
                                   MAC2STR(CMD_ID_REMOVETAB),
                                   MAC2STR(CMD_ID_ACTIVATETAB),
                                   MAC2STR(CMD_ID_LOADURL),
                                   MAC2STR(CMD_ID_SCROLL),
                                   MAC2STR(CMD_ID_MOUSECLICK),
                                   MAC2STR(CMD_ID_MOUSEMOVE),
                                   MAC2STR(CMD_ID_MOVECURSOR),
                                   MAC2STR(CMD_ID_GETSCROLL),
                                   MAC2STR(CMD_ID_SHOWMOUSECURSOR),
                                   MAC2STR(CMD_ID_HIDEMOUSECURSOR),
                                   MAC2STR(CMD_ID_ZOOM),
                                   MAC2STR(CMD_ID_GETZOOMFACTOR),
                                   MAC2STR(CMD_ID_HISTORYMOVE),
                                   MAC2STR(CMD_ID_RELOAD),
                                   MAC2STR(CMD_ID_STOPLOAD),
                                   MAC2STR(CMD_ID_FOCUS),
                                   MAC2STR(CMD_ID_BLUR),
                                   MAC2STR(CMD_ID_SHOW),
                                   MAC2STR(CMD_ID_GETWINDOWSIZE),
                                   MAC2STR(CMD_ID_GETCURSORPOSITION),
                                   MAC2STR(CMD_ID_GETTABTITLE),
                                   MAC2STR(CMD_ID_GETCERTIFICATEINFO),
                                   MAC2STR(CMD_ID_ISVIRTUALKEYBOARDVISIBLE),
                                   MAC2STR(CMD_ID_CREATETHUMBNAIL),
                                   MAC2STR(CMD_ID_CREATETHUMBNAILFORURL),
                                   MAC2STR(CMD_ID_SAVEBOOKMARK),
                                   MAC2STR(CMD_ID_MOVEBOOKMARK),
                                   MAC2STR(CMD_ID_DELETEBOOKMARK),
                                   MAC2STR(CMD_ID_GETALLBOOKMARKS),
                                   MAC2STR(CMD_ID_GETHISTORY),
                                   MAC2STR(CMD_ID_CLEARHISTORY),
                                   MAC2STR(CMD_ID_GETSPEEDDIALS),
                                   MAC2STR(CMD_ID_ADDSPEEDDIAL),
                                   MAC2STR(CMD_ID_MOVESPEEDDIAL),
                                   MAC2STR(CMD_ID_REMOVESPEEDDIAL),
                                   MAC2STR(CMD_ID_DIALOGCLOSED),
                                   MAC2STR(CMD_ID_GETSETTINGS),
                                   MAC2STR(CMD_ID_GETHIGHLIGHTEDURL),
                                   MAC2STR(CMD_ID_CHANGEPREFERENCES),
                                   MAC2STR(CMD_ID_ISFEATUREENABLED),
                                   MAC2STR(CMD_ID_CAPTURE_THUMBNAIL),
                                   MAC2STR(CMD_ID_THUMBNAIL_DATA),
                                   MAC2STR(CMD_ID_WINDOWRESIZETO),
                                   MAC2STR(CMD_ID_UPDATE_TITLE),
                                   MAC2STR(CMD_ID_DELETEHISTORY),
                                   MAC2STR(CMD_ID_CLEARCOOKIES),
                                   MAC2STR(CMD_ID_UPDATE_ICON),
                                   MAC2STR(CMD_ID_FOCUS_MAIN_WINDOW),
                                   MAC2STR(CMD_ID_GET_CURSOR_AND_SCROLL),
                                   MAC2STR(CMD_ID_VIDEO_FULLSCREEN),
                                   MAC2STR(CMD_ID_VIDEO_PLAYED),
                                   MAC2STR(CMD_ID_MODIFY_SPEEDDIAL),
                                   MAC2STR(CMD_ID_VIDEO_BACKWARD),
                                   MAC2STR(CMD_ID_VIDEOPLAYPAUSE),
                                   MAC2STR(CMD_ID_VIDEOSTOP),
                                   MAC2STR(CMD_ID_VIDEOFORWARD),
                                   MAC2STR(CMD_ID_VIDEO_ENDED),
                                   MAC2STR(CMD_ID_VIDEO_TIME_UPDATE),
                                   MAC2STR(CMD_ID_VIDEO_WAITING),
                                   MAC2STR(CMD_ID_VIDEO_PLAYING),
                                   MAC2STR(CMD_ID_VIDEO_PAUSED),
                                   MAC2STR(CMD_ID_VIDEO_SET_PERCENTAGE),
                                   MAC2STR(CMD_ID_GETSPEEDDIALBYURL),
                                   MAC2STR(CMD_ID_GET_VIRTUAL_KEYBOARD_STATUS),
                                   MAC2STR(CMD_ID_DESTROY_THUMBNAIL_WINDOW),
                                   MAC2STR(CMD_ID_VIDEO_EXIT_FULLSCREEN),
    };
    BrowserModeHiBrowser::Type getCmdType(std::string msg);
    std::string getValueByName(std::string msg, std::string name, const char* defaultValue);
    std::string getValueByName(std::string msg, std::string name, const std::string defaultValue);
    int getValueByName(std::string msg, std::string name, int defaultValue);
    bool getValueByName(std::string msg, std::string name, bool defaultValue);
    float getValueByName(std::string msg, std::string name, double defaultValue);

    class WindowInfo
    {
    public:
        std::string url;
        std::string title;
        double zoomFactor;
    };

    WindowInfo m_currentWindowInfo;

#if 1 //def DATABASE_IMPLEMENT
    std::shared_ptr<HistoryDBManager> m_historyDB;
    std::shared_ptr<BookmarkDBManager> m_bookmarkDB;
    std::shared_ptr<BookmarkDBManager> m_speeddialDB;
#else
    HistoryManager m_historys;
    BookmarkManager m_bookmarks;
    BookmarkManager m_speeddials;
#endif
private:

    void* m_library;
    SET_THUMBNAIL_WINDOW_ID m_setThumbnailWindowID;

    std::string m_oom_url;
    bool m_OOMEntered;
    bool m_thumbnailWindowCreated;
    bool m_UAChangedToDefault;
};

#endif
