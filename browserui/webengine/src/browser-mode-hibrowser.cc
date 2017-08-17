
#define DATABASE_IMPLEMENT 1
#include <json/json.h>
#include <dlfcn.h>
#include "browser-mode-hibrowser.h"
#include "browser-opera.h"
#include "browser-input.h"
#include "browser_debug.h"
#include "virtual-keyboard.h"
#include "configuration.h"
#include "hibrowser_type.h"
#include "browser_mouse.h"
#include "language.h"
#if 1 //def DATABASE_IMPLEMENT
#include "sqlite_mgr.h"
#include "history_db_manager.h"
#include "bookmark_db_mgr.h"
#endif
#include "graphics.h"
#include "async_browser_task.h"
#include "task_manager.h"
#include "NotImplemented.h"
#include <opera/messages/LoadURL.h>
#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include "media_source_status.h"

#define DEFAULT_UI_VIEW_WIDTH 1920
#define DEFAULT_UI_VIEW_HEIGHT 1080
#define THUMBNAIL_DEFAULT_WIDTH 373
#define THUMBNAIL_DEFAULT_HEIGHT 210
#define MAX_BOOKMARK_SIZE 6
#define MAX_SPEEDIAL_SIZE 36
#define BLANK_PAGE "about:blank"
#define UNDEFINED_URL "undefined"
#define THUMBNAIL_BOUNDS "1264x711"

const std::string UVA_BACKEND_LIB = "libuvabackend-dummy.so";
bool  keyboard_search_flag = false;
bool  isSpecialURL = false;
namespace {
bool has_alt_modifier(const opera::msg::KeyInput& key_input) {
  return key_input.key_modifiers & opera::msg::KeyInput::MODIFIER_ALT;
}
}

namespace {
    class HiBrowserActiveWindowGetter : public BrowserModeHiBrowser::ExampleInputInterceptor::ActiveWindowGetter {
    public:
        HiBrowserActiveWindowGetter( opera::Handle window_id )
            : m_active_window( window_id ) {}

        HiBrowserActiveWindowGetter( opera::Handle window_id , opera::Handle ui_id)
            : m_active_window( window_id), m_ui_window(ui_id){}
        virtual bool getActiveWindow( opera::Handle* window_id ) OPERA_OVERRIDE {
            *window_id = m_active_window;
            return true;
        }

        virtual bool setWindow( opera::Handle window_id , opera::Handle ui_id) {
            m_active_window = window_id;
            m_ui_window = ui_id;
            return true;
        }

        virtual bool getUiWindow(opera::Handle* ui_id) OPERA_OVERRIDE {
            if(m_ui_window.IsValid()){
                *ui_id = m_ui_window;
                return true;
            }
            return false;
        }


    private:
        opera::Handle m_active_window;
        opera::Handle m_ui_window;
    };
}

HiBrowserActiveWindowGetter* ActiveWindowGetter = NULL;

BrowserModeHiBrowser::ExampleInputInterceptor::ExampleInputInterceptor(BrowserImpl* browserImpl)
    : m_browserImpl(browserImpl), last_tv_time(0),key_repeat(false),m_videoFullscreen(false),active_window_getter_(NULL),m_hardware_cursor_has_event(false){
}

bool BrowserModeHiBrowser::ExampleInputInterceptor::InterceptKeyEvent(
    const opera::msg::KeyInput& key_input) {
    opera::Handle window_id;
    if (key_input.event_type == opera::msg::KeyInput::KEYDOWN){
        struct timespec now={0,0};
        clock_gettime(CLOCK_MONOTONIC, &now);
        long now_tv_time=now.tv_sec*1000+now.tv_nsec/1000000;
        if(now_tv_time-last_tv_time<100){
           BROWSER_LOG(INFO,"InterceptKeyEvent key_repeat=true");
           key_repeat=true;
        }else{
           BROWSER_LOG(INFO,"InterceptKeyEvent key_repeat=false");
           key_repeat=false;
        }
        last_tv_time=now_tv_time;
    }
    if (active_window_getter_.get()){
        active_window_getter_->getActiveWindow(&window_id);
        //window_id = main_window_vector.at(activeTabIndex).m_main_window;
        HisenseOpera& opera = browser().getHisenseOpera();
        if(active_window_getter_->getUiWindow(&m_UIWindow) && !(browser().getVirtualKeyboard().isShow()))
        {
            if( key_input.virtual_key == opera::msg::KeyInput::OMI_KEY_POWER)
            {
                browser().setUrl(BROWSER_MODE_OFF, "");
                return false;
            }
            if (m_videoFullscreen)
            {
               // return false;
            }
            //EnterKeyProc(key_input);
            browser().getVirtualKeyboard().EnterKeyProc(key_input);

            opera::Handle force_window = opera::Handle(-1);
            int multiplication = 4;
            switch( key_input.virtual_key )
            {
                case opera::msg::KeyInput::OMI_KEY_ENTER:
                case opera::msg::KeyInput::OMI_KEY_LEFT:
                case opera::msg::KeyInput::OMI_KEY_RIGHT:
                case opera::msg::KeyInput::OMI_KEY_UP:
                case opera::msg::KeyInput::OMI_KEY_DOWN:
                    if(BrowserModeHiBrowser::mouseShown){
                        handleKeyForVirtualMouse(key_input,window_id);
                        BROWSER_LOG(INFO,"InterceptKeyEvent OMI_KEY_ENTER here");
                    }
                    else{
                        force_window = m_UIWindow;
                        opera.sendKeyInput( key_input.event_type,
                                    key_input.virtual_key,
                                    opera::msg::KeyInput::MODIFIER_NONE,
                                    key_input.key_repeat,
                                    key_input.key_value,
                                    force_window );
                    }
                    break;
                case opera::msg::KeyInput::OMI_KEY_BACK:
                    force_window = m_UIWindow;
                    opera.sendKeyInput( key_input.event_type,
                                    key_input.virtual_key,
                                    opera::msg::KeyInput::MODIFIER_NONE,
                                    key_input.key_repeat,
                                    key_input.key_value,
                                    force_window );
                    break;
                case opera::msg::KeyInput::OMI_KEY_YELLOW:
                case opera::msg::KeyInput::OMI_KEY_BLUE:
                case opera::msg::KeyInput::OMI_KEY_MENU:
                    if(!m_videoFullscreen)
                    {
                         force_window = m_UIWindow;
                         opera.sendKeyInput( key_input.event_type,
                                key_input.virtual_key,
                                opera::msg::KeyInput::MODIFIER_NONE,
                                key_input.key_repeat,
                                key_input.key_value,
                                force_window );
                     }
                    break;
                case opera::msg::KeyInput::OMI_KEY_CHANNEL_DOWN:
                case opera::msg::KeyInput::OMI_KEY_CHANNEL_UP:
                    if(key_input.event_type == opera::msg::KeyInput::KEYDOWN && !m_videoFullscreen)
                    {
                        if(key_repeat)
                        {
                            multiplication = 16;
                        }
                        if (key_input.virtual_key == opera::msg::KeyInput::OMI_KEY_CHANNEL_UP)
                        {
                            scrollUp(window_id, multiplication);
                        }
                        else
                        {
                            scrollDown(window_id, multiplication);
                        }
                    }
                    break;
                case opera::msg::KeyInput::OMI_KEY_GREEN:
                case opera::msg::KeyInput::OMI_KEY_RED:
                    if (key_input.event_type == opera::msg::KeyInput::KEYDOWN && !m_videoFullscreen)
                    {
                        browser().getHisenseOpera().moveInHistory(window_id,
                            opera::msg::KeyInput::OMI_KEY_GREEN == key_input.virtual_key ? 1 :0);
                    }
                    break;

                case opera::msg::KeyInput::OMI_KEY_PLAY:
                {
                    if (key_input.event_type == opera::msg::KeyInput::KEYDOWN){
                        BROWSER_LOG(INFO, "video play key\n");
                        Json::Value o;
                        o["msg"] = getMessageStringValue(MESSAGE_VIDEO_PLAY);
                        Json::FastWriter writer;
                        opera.sendPlatformEvent(window_id, writer.write(o));
                    }
                    break;
                }
                case opera::msg::KeyInput::OMI_KEY_STOP:
                {
                    if (key_input.event_type == opera::msg::KeyInput::KEYDOWN){
                        BROWSER_LOG(INFO, "video stop key\n");
                        Json::Value o;
                        o["msg"] = getMessageStringValue(MESSAGE_VIDEO_STOP);
                        Json::FastWriter writer;
                        opera.sendPlatformEvent(window_id, writer.write(o));
                    }
                    break;
                }
                case opera::msg::KeyInput::OMI_KEY_PAUSE:
                {
                    if (key_input.event_type == opera::msg::KeyInput::KEYDOWN){
                        BROWSER_LOG(INFO, "video pause key\n");
                        Json::Value o;
                        o["msg"] = getMessageStringValue(MESSAGE_VIDEO_PAUSE);
                        Json::FastWriter writer;
                        opera.sendPlatformEvent(window_id, writer.write(o));
                    }
                    break;
                }
                case opera::msg::KeyInput::OMI_KEY_FAST_FWD:
                {
                    if (key_input.event_type == opera::msg::KeyInput::KEYDOWN){
                        BROWSER_LOG(INFO, "video foward key\n");
                        Json::Value o;
                        o["msg"] = getMessageStringValue(MESSAGE_VIDEO_FORWARD);
                        Json::FastWriter writer;
                        opera.sendPlatformEvent(window_id, writer.write(o));
                    }
                    break;
                }
                case opera::msg::KeyInput::OMI_KEY_REWIND:
                {
                    if (key_input.event_type == opera::msg::KeyInput::KEYDOWN){
                        BROWSER_LOG(INFO, "video backward key\n");
                        Json::Value o;
                        o["msg"] = getMessageStringValue(MESSAGE_VIDEO_BACKWARD);
                        Json::FastWriter writer;
                        opera.sendPlatformEvent(window_id, writer.write(o));
                    }
                    break;
                }
                default:
                    return false;
            }
            return true;
        }else if(browser().getVirtualKeyboard().isShow()){

            browser().getVirtualKeyboard().sendKeyInput(key_input);

//            opera.sendKeyInput( key_input.event_type,
//                                key_input.virtual_key,
//                                key_input.key_modifiers,
//                                key_input.key_repeat,
//                                key_input.key_value,
//                                browser().getVirtualKeyboard().getWindow());
            BROWSER_LOG(INFO,"send key to virtual keyboard!!!");

            return true;
        }
    }
    return false;
}
void BrowserModeHiBrowser::ExampleInputInterceptor::handleKeyForVirtualMouse(const opera::msg::KeyInput& Key,opera::Handle window_id)
{
     if (Key.event_type == opera::msg::KeyInput::KEYUP){
        return;
     }
     BROWSER_LOG(INFO,"handleKeyForVirtualMouse!!!");
     int scroll_when_less_than=20;
     int move_step =18;
     int scroll_step=300;
     int mouse_x, mouse_y;
     int scroll_x = -1;
     int scroll_y = -1;
     int MaxX = -1;
     int MaxY = -1;
     if(key_repeat)
     {
        move_step=100;
        scroll_step=800;
     }
     HisenseOpera& opera = browser().getHisenseOpera();
     BrowserMouse& mouse = browser().getBrowserMouse();
     mouse.getCoordinate( &mouse_x, &mouse_y );
     opera.getCurrentScroll( window_id, scroll_x, scroll_y );
     opera.getMaxScroll( window_id, MaxX, MaxY );
     switch( Key.virtual_key )
     {
         case opera::msg::KeyInput::OMI_KEY_ENTER:
         {
            opera.sendMouseInput(mouse_x, mouse_y, opera::msg::MouseInput::PRESS, opera::msg::MouseInput::LEFT);
            opera.sendMouseInput(mouse_x, mouse_y, opera::msg::MouseInput::RELEASE, opera::msg::MouseInput::LEFT);
            break;
         }
         case opera::msg::KeyInput::OMI_KEY_LEFT:
         {
            if(mouse_x>=scroll_when_less_than || scroll_x == 0)
            {
                BrowserMouse& mouse = browser().getBrowserMouse();
                if(mouse_x-move_step<=0){
                    mouse.setCoordinate( 0, mouse_y );
                    opera.sendMouseInput( 0, mouse_y, opera::msg::MouseInput::MOVE);
                }else{
                    mouse.setCoordinate( mouse_x-move_step, mouse_y );
                    opera.sendMouseInput(mouse_x-move_step, mouse_y, opera::msg::MouseInput::MOVE);
                }
            }else{
                opera.setScroll(window_id, scroll_x-scroll_step, scroll_y);
            }
            break;
         }
         case opera::msg::KeyInput::OMI_KEY_RIGHT:
         {
            if((mouse_x + scroll_when_less_than > 1920)&& (scroll_x < MaxX))
            {
                opera.setScroll(window_id, scroll_x+scroll_step, scroll_y);
            }else{
                BrowserMouse& mouse = browser().getBrowserMouse();
                if(mouse_x+move_step>=1920){
                    mouse.setCoordinate( 1920, mouse_y );
                    opera.sendMouseInput(1920, mouse_y, opera::msg::MouseInput::MOVE);
                }else{
                    mouse.setCoordinate( mouse_x+move_step, mouse_y );
                    opera.sendMouseInput(mouse_x+move_step, mouse_y, opera::msg::MouseInput::MOVE);
                }
            }
            break;
         }
         case opera::msg::KeyInput::OMI_KEY_UP:
         {
            if(mouse_y>scroll_when_less_than){
                BrowserMouse& mouse = browser().getBrowserMouse();
                if(mouse_y - move_step<=0){
                    mouse.setCoordinate( mouse_x, 0 );
                    opera.sendMouseInput(mouse_x, 0, opera::msg::MouseInput::MOVE);
                }else{
                    mouse.setCoordinate( mouse_x, mouse_y - move_step );
                    opera.sendMouseInput(mouse_x, mouse_y - move_step, opera::msg::MouseInput::MOVE);
                }
            }else{
                //opera.setScroll(window_id, scroll_x,scroll_y-scroll_step);
                if(existsHardwareCursor()){
                    BrowserModeHiBrowser::mouseShown=false;
                }else{
                    BrowserModeHiBrowser::mouseShown=false;
                    opera.hideCursor( window_id );
                }
                opera.setBounds(m_UIWindow, 0, 0, 1920, 1080);
                opera.show(m_UIWindow);
                opera.setStackAtTop(m_UIWindow);
                opera.focus(m_UIWindow);

                Json::Value o;
                o["msg"] = "showNavigation";
                Json::FastWriter write;
                opera.sendPlatformEvent( m_UIWindow, write.write(o));
                /*
                opera.sendKeyInput( Key.event_type,
                                    Key.virtual_key,
                                    opera::msg::KeyInput::MODIFIER_NONE,
                                    Key.key_repeat,
                                    Key.key_value,
                                    m_UIWindow );
                */
            }
            break;
         }
         case opera::msg::KeyInput::OMI_KEY_DOWN:
         {
            if((mouse_y+scroll_when_less_than > 1080)&&(scroll_y < MaxY)){
                opera.setScroll(window_id, scroll_x,scroll_y+scroll_step);
            }else{
                BrowserMouse& mouse = browser().getBrowserMouse();
                if(mouse_y + move_step>=1080){
                    mouse.setCoordinate( mouse_x, 1080 );
                    opera.sendMouseInput(mouse_x, 1080, opera::msg::MouseInput::MOVE);
                }else{
                   mouse.setCoordinate( mouse_x, mouse_y + move_step );
                   opera.sendMouseInput(mouse_x, mouse_y + move_step, opera::msg::MouseInput::MOVE);
                }
            }
            break;
         }
         default:
            break;
     }
}
bool BrowserModeHiBrowser::ExampleInputInterceptor::existsHardwareCursor()
{
   //TODO: set time out to reset this value if expired.
    return m_hardware_cursor_has_event;
}

bool BrowserModeHiBrowser::ExampleInputInterceptor::InterceptMouseEvent(
    const opera::msg::MouseInput& mouse_input)
{
    m_hardware_cursor_has_event = true;

    if(!m_receiver.IsValid())
        return false;

    browser().getVirtualKeyboard().MouseClickProc(mouse_input);

    #if 0
    std::string mouseRangeJson = browser().getConfiguration().getValueByKey("MouseRange");

    int minY(10);
    Json::Reader reader;
    Json::Value  value;
    bool parse_result = reader.parse(mouseRangeJson, value);
    if(parse_result)
    {
        minY =  value.get("MinY",10).asInt();
    }
    #else //for Test
    int minY(10);
    #endif
 //   BLOGV("Mouse Event: %s (%d,%d)",opera::msg::MouseInput::ToString(mouse_input.event_type).c_str(), mouse_input.x, mouse_input.y);
   // if (!m_videoFullscreen)
    {
        if (mouse_input.y <= minY)
        {
            if(mouseEvent_timeout.isRunning())
            {
                if(mouseEvent_timeout.isExpired())
                {
                    HisenseOpera& opera = browser().getHisenseOpera();
                    Json::Value o;
                    o["msg"] = getMessageStringValue(MESSAGE_TOP_MARGIN_REACHED);
                    o["x"] = mouse_input.x;
                    Json::FastWriter write;
                    opera.sendPlatformEvent( m_receiver, write.write(o));
                    mouseEvent_timeout.stop();
                    mouseEvent_timeout.start(500000);
                }
            }
            else
            {
                    HisenseOpera& opera = browser().getHisenseOpera();
                    Json::Value o;
                    o["msg"] = getMessageStringValue(MESSAGE_TOP_MARGIN_REACHED);
                    o["x"] = mouse_input.x;
                    Json::FastWriter write;
                    opera.sendPlatformEvent( m_receiver, write.write(o));
                    mouseEvent_timeout.start(500000);
            }
        }
        else if(mouseEvent_timeout.isRunning())
        {
             mouseEvent_timeout.stop();
        }
    }
    return false;
}

void BrowserModeHiBrowser::ExampleInputInterceptor::setActiveWindowGetter(
    ActiveWindowGetter* active_window_getter) {
  active_window_getter_.reset(active_window_getter);
}
void BrowserModeHiBrowser::ExampleInputInterceptor::scrollUp(const opera::Handle& window,int multiplication){
     HisenseOpera& opera = browser().getHisenseOpera();
     int currentX = -1;
     int currentY = -1;
     int MaxX = -1;
     int MaxY = -1;
     opera.getCurrentScroll( window, currentX, currentY );
     opera.getMaxScroll( window, MaxX, MaxY );
     BROWSER_LOG(INFO,"scrollUp:currentY=%d,MaxY=%d", currentY,MaxY);
     if(currentY>50*multiplication){
          opera.setScroll(window, currentX, currentY-50*multiplication);
     }else{
          opera.setScroll(window, currentX, 0);
     }

}
void BrowserModeHiBrowser::ExampleInputInterceptor::scrollDown(const opera::Handle& window,int multiplication){
     HisenseOpera& opera = browser().getHisenseOpera();
     int currentX = -1;
     int currentY = -1;
     int MaxX = -1;
     int MaxY = -1;
     opera.getCurrentScroll( window, currentX, currentY );
     opera.getMaxScroll( window, MaxX, MaxY );
     BROWSER_LOG(INFO,"scrollDown:currentY=%d,MaxY=%d", currentY,MaxY);
     if(currentY+50*multiplication<MaxY){
           opera.setScroll(window, currentX, currentY+50*multiplication);
      }else{
            opera.setScroll(window, currentX, MaxY);
      }

}

bool BrowserModeHiBrowser::mouseShown=false;
BrowserModeHiBrowser::BrowserModeHiBrowser( BrowserImpl* browserImpl )
    : BrowserMode( browserImpl )
    , m_firstEntered(true)
    , m_input_interceptor( NULL )
    , m_library(NULL)
    , m_OOMEntered(false)
    ,m_thumbnailWindowCreated(false)
    ,m_UAChangedToDefault(false)
{
    tab_index = 0;
    activeTabIndex = 0;
    beforeTabIndex = 0;
    BROWSER_LOG( INFO, "" );
}

BrowserModeHiBrowser::~BrowserModeHiBrowser()
{
    BROWSER_LOG( INFO, "" );
}

BrowserStatus BrowserModeHiBrowser::start()
{
    BROWSER_LOG(INFO, "");
    m_input_interceptor = new ExampleInputInterceptor( &browser() );
    if ( !m_input_interceptor ) {
        BROWSER_LOG(ERROR, "Failed to create input interceptor");
        return BrowserError;
    }

    MESSAGES_MAP_INIT();


#if 0 // useless for OperaSDK4.9
    std::string uva_back_lib = getenv("UVA_BACKEND");
    if( !uva_back_lib.empty() ) {
        BROWSER_LOG(ALWAYS, "UVA backend library : %s", uva_back_lib.c_str());
    } else {
        BROWSER_LOG(ERROR, "Failed to UVA backend library");
        uva_back_lib = UVA_BACKEND_LIB;
    }

    m_library = dlopen(uva_back_lib.c_str(), RTLD_LAZY);
    if (!m_library) {
        BROWSER_LOG(ERROR, "Failed to load uva backend library error : %s", dlerror());
    } else {
        m_setThumbnailWindowID = (SET_THUMBNAIL_WINDOW_ID)dlsym(m_library, "setThumbnailWindowId");
        if (!m_setThumbnailWindowID) {
            BROWSER_LOG(ERROR, "Failed to load setThumbnailWindowId function error : %s", dlerror());
        }
    }
#endif
    m_oom_url = browser().getConfiguration().getValueByKey("OOMUrl");
    return BrowserOK;
}

void BrowserModeHiBrowser::stop()
{
    BROWSER_LOG(INFO, "");
    if( m_input_interceptor ) {
        delete m_input_interceptor;
        m_input_interceptor = NULL;
    }

    HisenseOpera& opera = browser().getHisenseOpera();
    if ( m_ui_view.IsValid() )
    {
#if 0 //def HTML_IMPLEMENT
    opera.destroyHTMLView( m_ui_view );
#else
    opera.destroyWindow( m_ui_view );
#endif
    }

  for(int i=0; i < main_window_vector.size(); i++) {
    if ( main_window_vector.at(i).m_main_window.IsValid() )
        opera.destroyWindow( main_window_vector.at(i).m_main_window );
    }
    if ( m_thumbnail_window.IsValid() )
        opera.destroyWindow( m_thumbnail_window );
        m_thumbnailWindowCreated=false;

    m_firstEntered=true;
    main_window_vector.clear();
}

void BrowserModeHiBrowser::enter(MultiModeArgument *argument)
{
    BROWSER_LOG( INFO, "" );
    m_U_url = "";
    BrowserLanguage& language = browser().getLanguage();
    std::string country = browser().getConfiguration().getTVCountryID();
#if 1
    browser().getSqliteManager().start();
    m_historyDB.reset(new HistoryDBManager(&browser()));
    if (!m_historyDB.get()) {
        //return BrowserError;
    } else {
        m_historyDB->initDB();
    }

    BROWSER_LOG(ALWAYS, "country is %s",country.c_str());
    m_bookmarkDB.reset(new BookmarkDBManager(&browser()));
    if (!m_bookmarkDB.get()) {
        //return BrowserError;
    } else {
        std::string maxSize = browser().getConfiguration().getValueByKey("BookmarkMAXSize");
        int size = MAX_BOOKMARK_SIZE;
        if (!maxSize.empty()) {
            size = atoi(maxSize.c_str());
        }
        m_bookmarkDB->initDB(size, BookmarkDBManager::BOOKMARK,country);
    }

    m_speeddialDB.reset(new BookmarkDBManager(&browser()));
    if (!m_speeddialDB.get()) {
        //return BrowserError;
    } else {
        std::string maxSize = browser().getConfiguration().getValueByKey("SpeedDialMAXSize");
        int size = MAX_SPEEDIAL_SIZE;
        if (!maxSize.empty()) {
            size = atoi(maxSize.c_str());
        }

        m_speeddialDB->initDB(size, BookmarkDBManager::SPEEDDIAL,country);
    }
#endif
    Graphics& graphics = browser().getGraphics();
    graphics.requestFocus( true , false);

    m_OOMEntered = false;
    initCurrentWindowData();

#if 0 //!defined DATABASE_IMPLEMENT
    initHistory();
    initBookmark();
    initSpeeddial();
#endif

    BROWSER_LOG(ALWAYS, "country is %s",country.c_str());
    m_speeddialDB->setCountry(country);

    BrowserInput& browserInput = browser().getBrowserInput();
    //Disable physical mouse input and enable key input
    browserInput.enableKeyInput( true );
    browserInput.enableMouseInput( true );

    HisenseOpera& opera = browser().getHisenseOpera();

    if(main_window_vector.empty()) {
        BROWSER_LOG(ALWAYS, "~~~~~~~~~~~~enter create main window");
        TapsWondow m_window;
        activeTabIndex = main_window_vector.size();
        m_window.m_main_window = opera.createWindow(true,false);
        m_window.url = "";
        m_window.title = "";
        main_window_vector.push_back( m_window );

    } else {
        BROWSER_LOG(ALWAYS, "~~~~~~~~~~~~!!!!!main_window_vector is not empty~~~~~~~~~~~~!!!!!");
        return;
    }

    if( main_window_vector.size() == 0 ) {
        BROWSER_LOG( ALWAYS,"~~~~~~~~~~~~!!!!!Failed to creat window vector!!!!!~~~~~~~~~~~~");//´òÓ¡log
        return;
    }

    //There are chances that the opera failed to send DisplaySize event to the input helper
    //So we send the default value here(Better than nothing):
    int w = graphics.getWidth();
    int h = graphics.getHeight();
    browserInput.updateDisplaySize(w,h);

    std::string uaString = browser().getConfiguration().getValueByKey( "BrowserUIUA" );
    opera.setPref( main_window_vector.at(activeTabIndex).m_main_window, "user_agent", uaString );
    opera.setPref( main_window_vector.at(activeTabIndex).m_main_window, "default_background_color", "#ffffff" );
    opera.setPref( main_window_vector.at(activeTabIndex).m_main_window, "transparent", "false" );

    if(browser().getConfiguration().getTTSEnable())
    {
        opera.setPref(main_window_vector.at(activeTabIndex).m_main_window, "chromevox_enabled.override", "true");
    }
    else
    {
        opera.setPref(main_window_vector.at(activeTabIndex).m_main_window, "chromevox_enabled.override", "false");
    }

    std::string highlight_enabled =browser().getConfiguration().getValueByKey("TTSHighlightEnabled");
    //Default to false:
    BLOGI("TTSHighlightEnabled: %s", highlight_enabled.c_str());
    if(highlight_enabled.empty()){
        highlight_enabled = "false";
    }

    opera.setPref(main_window_vector.at(activeTabIndex).m_main_window, "accessibility_cursor_highlight_enabled", highlight_enabled);
    opera.setPref(main_window_vector.at(activeTabIndex).m_main_window, "accessibility_caret_highlight_enabled", highlight_enabled);
    opera.setPref(main_window_vector.at(activeTabIndex).m_main_window, "accessibility_focus_highlight_enabled", highlight_enabled);

    opera.setInputMethod( main_window_vector.at(activeTabIndex).m_main_window, opera::msg::SetInputMethod::OMI_EVENTS_INPUT_METHOD);

    BrowserRect_t rect = {0, 0, DEFAULT_UI_VIEW_WIDTH, DEFAULT_UI_VIEW_HEIGHT};

    std::string lang;
    if(!browser().getLanguage().BrowserLanguageGet(lang))
    {
        if(strcmp(lang.c_str(),"ar")>=0)
        {
            BROWSER_LOG(ALWAYS, "reset arabic fonts.");
            opera.setPref( main_window_vector.at(activeTabIndex).m_main_window, "font_family.cursive", "Frutiger LT Arabic 55 Roman" );
            opera.setPref( main_window_vector.at(activeTabIndex).m_main_window, "font_family.default", "Frutiger LT Arabic 55 Roman" );
            opera.setPref( main_window_vector.at(activeTabIndex).m_main_window, "font_family.fantasy", "Frutiger LT Arabic 55 Roman" );
            opera.setPref( main_window_vector.at(activeTabIndex).m_main_window, "font_family.fixed", "Frutiger LT Arabic 55 Roman" );
            opera.setPref( main_window_vector.at(activeTabIndex).m_main_window, "font_family.sans", "Frutiger LT Arabic 55 Roman" );
            opera.setPref( main_window_vector.at(activeTabIndex).m_main_window, "font_family.serif", "Frutiger LT Arabic 55 Roman" );
        }
        opera.setPref( main_window_vector.at(activeTabIndex).m_main_window, "http_accept_language", lang.c_str() );
        opera.setPref( main_window_vector.at(activeTabIndex).m_main_window, "intl.app_locale", lang.c_str() );
        //opera.setPref( m_thumbnail_window, "http_accept_language", lang );
        //opera.setPref( m_thumbnail_window, "intl.app_locale", lang );
    }
    else
    {
        opera.setPref( main_window_vector.at(activeTabIndex).m_main_window, "http_accept_language", "en-US" );
        opera.setPref( main_window_vector.at(activeTabIndex).m_main_window, "intl.app_locale", "en-US" );
        //opera.setPref( m_thumbnail_window, "http_accept_language", "en-US" );
        //opera.setPref( m_thumbnail_window, "intl.app_locale", "en-US" );
    }

    m_uiUrl = browser().getConfiguration().getValueByKey("UI_URL");

#if 0 //def WINDOW_TRANS
    m_ui_view = opera.createWindow(true, false, false, rect, m_main_window);
#else
    m_ui_view = opera.createWindow(false, false, rect);
    opera.disableScreenReaderHighlight(m_ui_view);
#endif
    opera.setInputMethod( m_ui_view, opera::msg::SetInputMethod::OMI_EVENTS_INPUT_METHOD);
    opera.setPref( m_ui_view, "transparent", "true" );
    opera.loadURL( m_ui_view, m_uiUrl);

    // enter virtual keyboard
    browser().getVirtualKeyboard().enter( main_window_vector.at(activeTabIndex).m_main_window);
    browser().getVirtualKeyboard().setTargetWindows(main_window_vector.at(activeTabIndex).m_main_window,m_ui_view);
    //send window id to input interceptor
    ActiveWindowGetter = new HiBrowserActiveWindowGetter(main_window_vector.at(activeTabIndex).m_main_window,m_ui_view);
    if ( m_input_interceptor )
    {
        m_input_interceptor->setActiveWindowGetter( ActiveWindowGetter);
        m_input_interceptor->setPlatformEventReceiver(m_ui_view);
        m_input_interceptor->setVideoFullscreen(false);
        browserInput.setEventsIntercept( m_input_interceptor );
    }

    opera.AddEventHandler( &BrowserModeHiBrowser::loadingStarted, this );
    opera.AddEventHandler( &BrowserModeHiBrowser::loadingFinished, this );
    opera.AddEventHandler( &BrowserModeHiBrowser::onPlatformMessage,this);
    opera.AddEventHandler( &BrowserModeHiBrowser::onUrlChange, this);
    opera.AddEventHandler( &BrowserModeHiBrowser::onLoadingProgress, this);
    opera.AddEventHandler( &BrowserModeHiBrowser::BrowserMemoryEvent , this );
    opera.AddEventHandler( &BrowserModeHiBrowser::onTitleWasSet, this);
    opera.AddEventHandler( &BrowserModeHiBrowser::onFaviconWasSet, this);

    opera.resume();

}

void BrowserModeHiBrowser::leave()
{
    BROWSER_LOG(INFO, "");
    HisenseOpera& opera = browser().getHisenseOpera();
    BROWSER_LOG(INFO, "main_window_vector size is %d\n", main_window_vector.size());
    for (int i = 0; i < main_window_vector.size(); i++)
    {
        opera.destroyWindow( main_window_vector.at(i).m_main_window );
        BROWSER_LOG(INFO, "i is %d\n",i);
    }
    main_window_vector.clear();
    #if 1
    browser().getSqliteManager().stop();
    #endif
 #if 0 //!defined DATABASE_IMPLEMENT
    saveHistory();
    saveBookmark();
    saveSpeeddial();
 #endif

    BrowserInput& browserInput = browser().getBrowserInput();
    //Disable physical mouse input and key input
    browserInput.enableKeyInput( false );
    browserInput.enableMouseInput( false );

    //opera.setPageZoom( m_main_window, 1 );
    //opera.setPageZoom( m_thumbnail_window, 1 );

    opera.pause();
    opera.removeAllEventListeners();
    browser().getVirtualKeyboard().leave();

#if 0 //def HTML_IMPLEMENT
    opera.destroyHTMLView( m_ui_view );
#else
    opera.destroyWindow( m_ui_view );
#endif
    
    opera.destroyWindow( m_thumbnail_window );
    m_thumbnailWindowCreated=false;
    browserInput.setEventsIntercept(NULL);

    opera.clearDiskCache();

    m_firstEntered=true;
   
    int count = 0;
    while(lo::dtvaccess::SourceStatus::getInstance()->resourcePossessed())
    {
        BROWSER_LOG( ALWAYS, "~~~~~~~~~~~media player resource possessed,count=%d",count );
        if(count > 20)
        {
            BROWSER_LOG(ALWAYS,"~~~~~~~~~~~media player resource possessed,wait time out.\n");
            break;
        }
        usleep(100*1000);
        count++;
    }
    Graphics& graphics = browser().getGraphics();
    graphics.lostFocus();
}

void BrowserModeHiBrowser::BrowserMemoryEvent( opera::evt::BrowserMemoryEvent event)
{
     switch( event.state )
    {
        case opera::evt::BrowserMemoryEvent::OK:
            BROWSER_LOG( ALWAYS, "~~~~~~~~~~~Enough Browser Memory!!!" );
            break;
        case opera::evt::BrowserMemoryEvent::LOW:
            BROWSER_LOG( ERROR, "~~~~~~~~~~~~Low Browser Memory!!!" );
            break;
        case opera::evt::BrowserMemoryEvent::OOM:
            BROWSER_LOG( ERROR, "~~~~~~~~~~~~Browser Memory OOM!!!" );
            break;
        default:
            break;
    }
}

void BrowserModeHiBrowser::createThumbnailWindow(){
    BROWSER_LOG( ALWAYS,"create thumbnail window");
    HisenseOpera& opera = browser().getHisenseOpera();
    std::string uaString = browser().getConfiguration().getValueByKey( "BrowserUIUA" );
    BrowserRect_t rect = {0, 0, DEFAULT_UI_VIEW_WIDTH, DEFAULT_UI_VIEW_HEIGHT};
#if 0 //def WINDOW_TRANS
    m_thumbnail_window = opera.createWindow(true, false, false, rect, opera::Handle( -1));
#else
    m_thumbnail_window = opera.createWindow(false, false, rect, opera::Handle( -1));
    opera.disableScreenReaderHighlight(m_thumbnail_window);
#endif
    opera.setPref( m_thumbnail_window, "content_resolution", THUMBNAIL_BOUNDS );
    opera.setPref( m_thumbnail_window, "user_agent", uaString );
    //opera.setPref( m_thumbnail_window, "transparent", "false" );

    //Set thumbnail window id to UVA backend
    if (m_setThumbnailWindowID)
        m_setThumbnailWindowID(m_thumbnail_window.internal_id());

    //set multi-language
    std::string lang;
    if(!browser().getLanguage().BrowserLanguageGet(lang))
    {
        opera.setPref( m_thumbnail_window, "http_accept_language", lang.c_str() );
        opera.setPref( m_thumbnail_window, "intl.app_locale", lang.c_str() );
    }
    else
    {
        opera.setPref( m_thumbnail_window, "http_accept_language", "en-US" );
        opera.setPref( m_thumbnail_window, "intl.app_locale", "en-US" );
    }
    opera.setStackAtBottom( m_thumbnail_window );
}

void BrowserModeHiBrowser::setUrl( std::string url )
{
    BROWSER_LOG( INFO, "url = %s", url.c_str() );
    if (url.empty())
        return;
    m_U_url=url;
    if(!m_firstEntered){
    HisenseOpera& opera = browser().getHisenseOpera();
    main_window_vector.at(activeTabIndex).url = url;
    opera.loadURL( main_window_vector.at(activeTabIndex).m_main_window, url );
    }
}

void BrowserModeHiBrowser::processOOM()
{
    BROWSER_LOG(ERROR,"processOOM !!!!!!!!!!!!!!!!!");
    system("top -bn 1");
    if(!m_OOMEntered) {
        HisenseOpera& opera = browser().getHisenseOpera();
        if(m_thumbnailWindowCreated){
            BROWSER_LOG(ERROR,"OOM   destroy thumbnail window !");
            opera.stopLoading(m_thumbnail_window);
            opera.destroyWindow( m_thumbnail_window );
            m_thumbnailWindowCreated=false;
        }else{
            BROWSER_LOG(ERROR,"OOM   destroy main window and show oom_html!");
            opera.stopLoading(main_window_vector.at(activeTabIndex).m_main_window);
            opera.destroyWindow( main_window_vector.at(activeTabIndex).m_main_window );
            opera.setBounds(m_ui_view, 0, 0, 1920, 1080);
            opera.loadURL(m_ui_view, m_oom_url);
            m_OOMEntered = true;
        }
    }
}

void BrowserModeHiBrowser::onTitleWasSet(opera::evt::TitleWasSet event)
{
    if(event.id == main_window_vector.at(activeTabIndex).m_main_window)
    {
        main_window_vector.at(activeTabIndex).title = event.title;
        BROWSER_LOG(INFO, "onTitleWasSet: event.title is %s", event.title.c_str());
        //m_historyDB->modifyTitle(m_main_url, m_main_title);
        HisenseOpera& opera = browser().getHisenseOpera();
        Json::Value o;
        o["msg"] ="titleupdated";
        o["tab_id"] = activeTabIndex;
        o["title"] = main_window_vector.at(activeTabIndex).title;
        Json::FastWriter writer;
        opera.sendPlatformEvent(m_ui_view,writer.write(o));
    }
}

void BrowserModeHiBrowser::onFaviconWasSet(opera::evt::FaviconWasSet event)
{
    if(event.id == main_window_vector.at(activeTabIndex).m_main_window)
    {
        std::string icon = event.icons[0].icon_url;
        BROWSER_LOG(INFO, "onFaviconWasSet: event.icon_url is %s", event.icons[0].icon_url.c_str());
        m_historyDB->modifyICON(main_window_vector.at(activeTabIndex).url, icon);
    }
}

void BrowserModeHiBrowser::loadingStarted( opera::evt::LoadingStarted event )
{
    if (event.window_id.IsView())
        return;

    BROWSER_LOG(ALWAYS, "Loading started window: %s url : %s", event.window_id.str().c_str(), event.url.c_str());
    HisenseOpera& opera = browser().getHisenseOpera();

    // HTML view loading started on the parent window
    if  ( event.window_id == main_window_vector.at(activeTabIndex).m_main_window &&
        ( event.url.find( UNDEFINED_URL ) == std::string::npos ) ) {
        m_currentWindowInfo.url = event.url;

        Json::Value o;
        o["msg"] = getMessageStringValue(MESSAGE_LOADING_STARTED);
        o["tab_id"] = activeTabIndex;
        o["url"] = event.url;
        Json::FastWriter write;
        opera.sendPlatformEvent( m_ui_view , write.write(o));

        main_window_vector.at(activeTabIndex).title = event.url;
        m_main_url=event.url;
    }
}

void BrowserModeHiBrowser::loadingFinished( opera::evt::LoadingFinished event )
{
    if (event.window_id.IsView())
        return;

    HisenseOpera& opera = browser().getHisenseOpera();
    std::string status_str;
    Json::Value o;
    Json::FastWriter write;
    switch (event.status) {
        case opera::evt::LoadingFinished::LOAD_ABORTED:
            BROWSER_LOG(ERROR, "LOAD_ABORTED");
            status_str = "aborted";
            if(0 && event.window_id == m_thumbnail_window)
            {
                Json::Value o;
                o["msg"] = getMessageStringValue(MESSAGE_CREATE_THUMBNAIL);
                o["url"] = m_thumbnail_url;
                o["thumbnail"] = "default";
                o["title"] = "default";
                Json::FastWriter write;
                opera.sendPlatformEvent(m_ui_view , write.write(o));
            }
            break;
        case opera::evt::LoadingFinished::LOAD_SUCCEEDED:
            // HTML view loading finished on the parent window
            if ( event.window_id == main_window_vector.at(activeTabIndex).m_main_window &&
                ( event.url.find( UNDEFINED_URL ) == std::string::npos ) ) {
                BROWSER_LOG(INFO, "m_main_window loading finished.");
                o["msg"] = getMessageStringValue(MESSAGE_LOADING_FINISHED);
                o["type"] = "succeeded";
                //o["tab_id"] = event.window_id.internal_id();
                //use sigle window,retrun 0 always
                o["tab_id"] = activeTabIndex;
                opera.sendPlatformEvent( m_ui_view , write.write(o));
            }
            else if ( event.window_id == m_thumbnail_window)
            {
                o["msg"] = getMessageStringValue(MESSAGE_CAPTURE_THUMBNAIL);
                o["url"] = m_thumbnail_url;
                o["width"] = m_thumbnail_width;
                o["height"] = m_thumbnail_height;
                opera.sendPlatformEvent( m_thumbnail_window, write.write(o));
            }
            else{
                if(event.window_id == m_ui_view && event.url.find( m_uiUrl ) != std::string::npos){
                   if(m_firstEntered){
                      if(!m_U_url.empty()){
                        Json::Value o;
                        o["msg"] = "loadHomepageURL";
                        o["url"] = m_U_url;
                        Json::FastWriter writer;
                        opera.sendPlatformEvent(m_ui_view, writer.write(o));
                      }else{
                        Json::Value o;
                        o["msg"] = "loadHomepageURL";
                        o["url"] = "about:blank";
                        Json::FastWriter writer;
                        opera.sendPlatformEvent(m_ui_view, writer.write(o));
                        //opera.show(m_ui_view);
                      }
                      opera.show(main_window_vector.at(activeTabIndex).m_main_window);
                      opera.setStackAtTop( m_ui_view );
                      //opera.setStackAtBottom( m_thumbnail_window );
                      opera.focus( main_window_vector.at(activeTabIndex).m_main_window );
                      m_firstEntered=false;
                   }
                }
            }
            status_str = "succeeded";
            break;
        case opera::evt::LoadingFinished::LOAD_FAILED:
            if(event.window_id == m_thumbnail_window)
            {
                Json::Value o;
                o["msg"] = getMessageStringValue(MESSAGE_CREATE_THUMBNAIL);
                o["url"] = m_thumbnail_url;
                o["thumbnail"] = "default";
                o["title"] = "default";
                Json::FastWriter write;
                opera.sendPlatformEvent(m_ui_view , write.write(o));
            } else if ( event.window_id == main_window_vector.at(activeTabIndex).m_main_window &&
                ( event.url.find( UNDEFINED_URL ) == std::string::npos ) ){
                o["msg"] = "loadingfailed";
                opera.sendPlatformEvent( m_ui_view , write.write(o));
            }
            status_str = "failed";
            break;
        default:
            status_str = "other";
    }

    BROWSER_LOG(ALWAYS, "Loading finished (%s) window: %s %s", status_str.c_str(), event.window_id.str().c_str(), event.url.c_str());
}

void BrowserModeHiBrowser::onPlatformMessage( opera::evt::PlatformMessage event )
{
    if(event.message == "exitCurrentMode") {
        browser().getTaskManager().postTask( hisense::AsyncBrowserTask::create(&browser(),"pause"));
        return;
    }
    Type _type = getCmdType(event.message);
    BLOGV( "Platform message Handle : %s, Command:%s", event.window_id.str().c_str(), cmdString[_type] );
    HisenseOpera& opera = browser().getHisenseOpera();
    
    switch(_type)
    {
        case CMD_ID_AUTHENTICATIONRECEIVED:
            break;
        case CMD_ID_ADDTAB:
        {
            std::string url;
            if (main_window_vector.size() >= MAX_NUM_TABS)
            {
                BROWSER_LOG(ALWAYS, "no more tabs\n");
                return;
            }
            Json::Value o;
            Json::FastWriter writer;
    
            TapsWondow NewWindow;
            NewWindow.m_main_window = opera.createWindow(false,false);
            opera.setInputMethod( NewWindow.m_main_window, opera::msg::SetInputMethod::OMI_EVENTS_INPUT_METHOD);
            std::string uaString = browser().getConfiguration().getValueByKey( "BrowserUIUA" );
            opera.setPref( NewWindow.m_main_window, "user_agent", uaString );
            opera.setPref( NewWindow.m_main_window, "default_background_color", "#ffffff" );
            opera.setPref( NewWindow.m_main_window, "transparent", "true" );  
            url = getValueByName(event.message, "url", "");
            NewWindow.url = url;
            o["msg"] = getMessageStringValue(MESSAGE_EVENT_ADDED_TAB);
            o["tab_id"] = main_window_vector.size();
            o["url"] = getValueByName(event.message, "url", "");
            opera.sendPlatformEvent( m_ui_view, writer.write(o) );
            main_window_vector.push_back( NewWindow );
            main_window_vector.at(tab_index).title = "";
            opera.hide(NewWindow.m_main_window);
            /*opera.show(m_ui_view);
            opera.focus(m_ui_view);
            opera.setStackAtTop(m_ui_view);
            */
            break;
        }
        case CMD_ID_REMOVETAB:
        {
            Json::Value o;
            Json::FastWriter writer;
            BROWSER_LOG( ALWAYS,"Remove tab start!");
            if( main_window_vector.size() == 1 ) 
            { 
                return; 
            }
            int index = getValueByName(event.message, "id", 0);
            if( activeTabIndex == index )
            {
                BROWSER_LOG( ALWAYS,"activeTabIndex is %d\n",activeTabIndex);
                BROWSER_LOG( ALWAYS,"main_window_vector is %d\n",main_window_vector.size());
                opera::Handle destroyWindow = main_window_vector.at(activeTabIndex).m_main_window;
                std::vector<TapsWondow>::iterator it = main_window_vector.begin() + index;
                main_window_vector.erase(it);
                opera.destroyWindow( destroyWindow);
                BROWSER_LOG( ALWAYS,"main_window_vector is %d\n",main_window_vector.size());
                o["msg"] = getMessageStringValue(MESSAGE_EVENT_REMOVE_TAB);
                o["id"] = index;
                opera.sendPlatformEvent( m_ui_view, writer.write(o) );
    
                BROWSER_LOG( ALWAYS,"Remove tab end");
            }
            break;
        }
        case CMD_ID_ACTIVATETAB:
        {   
            Json::Value o;
            Json::FastWriter writer;
            BROWSER_LOG( ALWAYS,"Activated tab start!");//´òÓ¡log
            activeTabIndex = getValueByName(event.message, "id", 0);
            BROWSER_LOG( ALWAYS,"activeTabIndex is %d",activeTabIndex);
            o["msg"] = getMessageStringValue(MESSAGE_EVENT_TAB_ACTIVATED);
            o["id"] = activeTabIndex;
            opera.show(main_window_vector.at(activeTabIndex).m_main_window);
            opera::Handle m_window = main_window_vector.at(activeTabIndex).m_main_window;
            ActiveWindowGetter->setWindow(m_window,m_ui_view);
            browser().getVirtualKeyboard().setTargetWindows(main_window_vector.at(activeTabIndex).m_main_window,m_ui_view);
            if ((main_window_vector.size() > 1) 
                && (activeTabIndex != beforeTabIndex) 
                && (beforeTabIndex != main_window_vector.size()))
            {
                BROWSER_LOG( ALWAYS,"hide beforeTabIndex!");
                opera.hide(main_window_vector.at(beforeTabIndex).m_main_window);
            }
            BROWSER_LOG( ALWAYS,"hide beforeTabIndex end!");
            beforeTabIndex = activeTabIndex;
            opera.sendPlatformEvent( m_ui_view, writer.write(o) );
            BROWSER_LOG( ALWAYS,"Activated tab end!");
            break;
        }
        case CMD_ID_LOADURL:
        {
            std::string url;
            url = getValueByName(event.message, "url", "");
            if(url.length() > 0)
            {
                opera.loadURL(main_window_vector.at(activeTabIndex).m_main_window, url);
            }
            break;
        }
        case CMD_ID_SCROLL:
        {
            int x = getValueByName(event.message, "x", 0);
            int y = getValueByName(event.message, "y" , 0);
            opera.setScroll(main_window_vector.at(activeTabIndex).m_main_window, x, y);
            break;
        }
        case CMD_ID_MOUSECLICK:
        {
            int x = getValueByName(event.message, "x", 0);
            int y = getValueByName(event.message, "y", 0);
            opera.sendMouseInput(x, y, opera::msg::MouseInput::PRESS, opera::msg::MouseInput::LEFT);
            opera.sendMouseInput(x, y, opera::msg::MouseInput::RELEASE, opera::msg::MouseInput::LEFT);
            break;
        }
        case CMD_ID_MOUSEMOVE:
        {
            int x = getValueByName(event.message, "x", 0);
            int y = getValueByName(event.message, "y", 0);

            BrowserMouse& mouse = browser().getBrowserMouse();
            mouse.setCoordinate( x, y );

            opera.sendMouseInput(x, y, opera::msg::MouseInput::MOVE);
            break;
        }
        case CMD_ID_MOVECURSOR:
        {
            int x = getValueByName(event.message, "x", 0);
            int y = getValueByName(event.message, "y", 0);
            BrowserMouse& mouse = browser().getBrowserMouse();
            mouse.setCoordinate( x, y );

            opera.sendMouseInput(x, y, opera::msg::MouseInput::MOVE);
            break;
        }
        case CMD_ID_GETSCROLL:
        {
            int currentX = -1;
            int currentY = -1;
            int MaxX = -1;
            int MaxY = -1;
            opera.getCurrentScroll( main_window_vector.at(activeTabIndex).m_main_window, currentX, currentY );
            opera.getMaxScroll( main_window_vector.at(activeTabIndex).m_main_window, MaxX, MaxY );

            Json::Value o;
            Json::FastWriter writer;
            if( event.window_id == m_ui_view ){
                o["msg"] = getMessageStringValue(MESSAGE_GET_SCROLL);
                o["type"] = "set";
                o["scroll_x"] = currentX;
                o["scroll_y"] = currentY;
                o["max_scroll_x"] = MaxX;
                o["max_scroll_y"] = MaxY;
                opera.sendPlatformEvent( m_ui_view, writer.write(o) );
            }
            break;//"getScroll"
        }
        case CMD_ID_SHOWMOUSECURSOR:
        {
            setMouseStatus(true);
            opera.showCursor( main_window_vector.at(activeTabIndex).m_main_window );
            break;
        }
        case CMD_ID_HIDEMOUSECURSOR:
        {
            //Don't hide cursor if hardware cursor exists
            if(m_input_interceptor->existsHardwareCursor()){
                setMouseStatus(false);
                BLOGV("Hardware Cursor exists, ignore CMD_ID_HIDEMOUSECURSOR");
                break;
            }
            if(hideCursor_timeout.isRunning()){
                if(hideCursor_timeout.isExpired()){
                   setMouseStatus(false);
                   opera.hideCursor( main_window_vector.at(activeTabIndex).m_main_window );
                   hideCursor_timeout.stop();
                   hideCursor_timeout.start(1000000);
                }
            }else{
                setMouseStatus(false);
                opera.hideCursor( main_window_vector.at(activeTabIndex).m_main_window );
                hideCursor_timeout.start(1000000);
            }
            break;
        }
        case CMD_ID_ZOOM:
        {
            double zoom = getValueByName(event.message, "zoomlevel", (double)0);
            if (opera.setPageZoom( main_window_vector.at(activeTabIndex).m_main_window, zoom ) == BrowserOK)
                m_currentWindowInfo.zoomFactor = zoom;
            break;
        }
        case CMD_ID_GETZOOMFACTOR:
        {
            Json::Value o;
            Json::FastWriter writer;
            o["msg"] = getMessageStringValue(MESSAGE_GET_ZOOM);
            o["zoomFactor"] = m_currentWindowInfo.zoomFactor / 1.5;
            opera.sendPlatformEvent(m_ui_view, writer.write(o));
            break;//"getZoomFactor"
        }
        case CMD_ID_HISTORYMOVE:
        {
            int direction = getValueByName(event.message, "direction", 0);
            opera.moveInHistory(main_window_vector.at(activeTabIndex).m_main_window, direction, 1);
            break;
        }
        case CMD_ID_RELOAD:
        {
            opera.reload(main_window_vector.at(activeTabIndex).m_main_window);
            break;
        }
        case CMD_ID_STOPLOAD:
        {
            opera.stopLoading(main_window_vector.at(activeTabIndex).m_main_window);
            break;
        }
        case CMD_ID_FOCUS:
        {
            //opera.show(m_ui_view);
            opera.focus(m_ui_view);
            break;
        }
        case CMD_ID_BLUR:
        {
            if(!m_OOMEntered)
            {
                opera.hide(m_ui_view);
                if(!browser().getVirtualKeyboard().isShow())
                {
                    opera.focus(main_window_vector.at(activeTabIndex).m_main_window);
                }
            }
            break;
        }
        case CMD_ID_SHOW:
        {
            opera.show(m_ui_view);
            opera.setStackAtTop(m_ui_view);
            break;
        }
        case CMD_ID_FOCUS_MAIN_WINDOW:
        {
            opera.focus(main_window_vector.at(activeTabIndex).m_main_window);
            break;
        }
        case CMD_ID_GETWINDOWSIZE:
        {
            std::string resolution = opera.getPref( main_window_vector.at(activeTabIndex).m_main_window, "content_resolution" );
            std::size_t found = resolution.find("x");
            std::string width = resolution.substr(0,found);
            std::string height = resolution.substr(found + 1 ,resolution.length());

            Json::Value o;
            o["msg"] = getMessageStringValue(MESSAGE_GET_WINDOW_SIZE);
            o["width"] = atoi(width.c_str());
            o["height"] = atoi(height.c_str());
            Json::FastWriter writer;
            opera.sendPlatformEvent(m_ui_view, writer.write(o));
            break;//"getWindowSize"
        }
        case CMD_ID_GETCURSORPOSITION:
        {
            int x, y;
            BrowserMouse& mouse = browser().getBrowserMouse();
            mouse.getCoordinate( &x, &y );

            Json::Value o;
            o["msg"] = getMessageStringValue(MESSAGE_GET_CUROSR_POSITON);
            o["x"] = x;
            o["y"] = y;
            Json::FastWriter writer;
            opera.sendPlatformEvent(m_ui_view, writer.write(o));

            break;//"getCursorPosition"
        }
        case CMD_ID_GET_CURSOR_AND_SCROLL:
        {
            int mouse_x, mouse_y;
            int currentX = -1;
            int currentY = -1;
            int MaxX = -1;
            int MaxY = -1;
            BrowserMouse& mouse = browser().getBrowserMouse();
            mouse.getCoordinate( &mouse_x, &mouse_y );
            opera.getCurrentScroll( main_window_vector.at(activeTabIndex).m_main_window, currentX, currentY );
            opera.getMaxScroll( main_window_vector.at(activeTabIndex).m_main_window, MaxX, MaxY );
            Json::Value o;
            o["msg"] = "getCursorAndScroll";
            o["x"] = mouse_x;
            o["y"] = mouse_y;
            o["scroll_x"]=currentX;
            o["scroll_y"]=currentY;
            o["max_scroll_x"]=MaxX;
            o["max_scroll_y"]=MaxY;
            Json::FastWriter writer;
            opera.sendPlatformEvent(m_ui_view, writer.write(o));
            break;
        }
        case CMD_ID_GETTABTITLE:
        {
            ///TODO:?
            Json::Value o;
            o["msg"] = getMessageStringValue(MESSAGE_GET_TAB_TITLE);
            o["tab_id"] = getValueByName(event.message, "id", 0);
            o["title"] = main_window_vector.at(activeTabIndex).title;
            Json::FastWriter writer;
            opera.sendPlatformEvent(m_ui_view, writer.write(o));
            break;//"getTabTitle"
        }
        case CMD_ID_GETCERTIFICATEINFO:
            break;//"getCertificateInfo"
        case CMD_ID_ISVIRTUALKEYBOARDVISIBLE:
            break;//"isvitualKeyboardVisible"
        case CMD_ID_CREATETHUMBNAIL:
        {
#if 0
            m_thumbnail_url.clear();
            m_thumbnail_url = m_main_url;
            if ( m_thumbnail_url.empty() || m_thumbnail_url.find(UNDEFINED_URL) == std::string::npos )
            {
                break;
            }
            m_thumbnail_width = getValueByName(event.message, "width", THUMBNAIL_DEFAULT_WIDTH);
            m_thumbnail_height = getValueByName(event.message, "height", THUMBNAIL_DEFAULT_HEIGHT);

            opera.stopLoading(m_thumbnail_window);
            opera.setStackAtBottom(m_thumbnail_window);
            opera.loadURL(m_thumbnail_window, m_thumbnail_url);
#endif

            break;//"createThumbnail"
        }
        case CMD_ID_CREATETHUMBNAILFORURL:
        {
            if (! m_thumbnailWindowCreated ){
                BROWSER_LOG( ALWAYS,"m_thumbnail_window is not valid , create thumbnail window");
                createThumbnailWindow();
                m_thumbnailWindowCreated=true;
            }
            m_thumbnail_url.clear();
            m_thumbnail_url = getValueByName(event.message, "url", "");
            m_thumbnail_width = getValueByName(event.message, "width", THUMBNAIL_DEFAULT_WIDTH);
            m_thumbnail_height = getValueByName(event.message, "height", THUMBNAIL_DEFAULT_HEIGHT);

            opera.stopLoading(m_thumbnail_window);
            opera.setStackAtBottom(m_thumbnail_window);
            opera.loadURL(m_thumbnail_window, m_thumbnail_url);
            break;//"createThumbnailForUrl"
        }
        case CMD_ID_DESTROY_THUMBNAIL_WINDOW:
        {
             if ( m_thumbnailWindowCreated){
                     opera.stopLoading(m_thumbnail_window);
                     opera.destroyWindow( m_thumbnail_window );
                     m_thumbnailWindowCreated=false;
                     BROWSER_LOG( ALWAYS,"destroy thumbnail window");
                }
             break;//"destroyThumbnailWindow"
        }
        case CMD_ID_SAVEBOOKMARK:
        {
            std::string url = getValueByName(event.message, "url", "");
            std::string title = getValueByName(event.message , "title", "");
            int res;
        #if 1 //def  DATABASE_IMPLEMENT
            if (m_speeddialDB->find(url) == 0) {
                res = m_bookmarkDB->addBookmark(url, title, "");
                Json::Value o;
                o["msg"] = "saveBookmark";
                o["url"] = url;
                o["title"] = title;
                o["result"] = res;
                Json::FastWriter writer;
                opera.sendPlatformEvent(m_ui_view,writer.write(o));
            } else {
                BROWSER_LOG( ERROR, "Bookmark already exists in speeddials");
            }
        #else
            if(m_speeddials.find(url) == -1){
                res = m_bookmarks.addBookmark(url , title,"");
                saveBookmark();
                Json::Value o;
                o["msg"] = "saveBookmark";
                o["url"] = url;
                o["title"] = title;
                o["result"] = res;
                Json::FastWriter writer;
                opera.sendPlatformEvent(m_ui_view,writer.write(o));
            } else {
                BROWSER_LOG( INFO, "Bookmark already exists in speeddials");
            }
        #endif

            break;//"saveBookmark"
        }
        case CMD_ID_MOVEBOOKMARK:
        {
            #if 0
            std::string url = getValueByName( event.message, "url","");
            int position = getValueByName(event.message,"position",0);
            m_bookmarks.moveBookmark(url,position);
            saveBookmark();
            #endif
            break;//"moveBookmark"
        }
        case CMD_ID_DELETEBOOKMARK:
        {
            std::string url = getValueByName( event.message , "url" , "");
            bool result = false;
        #if 1 //def DATABASE_IMPLEMENT
            result = m_bookmarkDB->removeBookmark(url);
        #else
            result = m_bookmarks.removeBookmark(url);
            saveBookmark();
        #endif
            Json::Value o;
            o["msg"] = "deleteBookmark";
            o["url"] = url;
            o["result"] = result ? 0 : 1;
            Json::FastWriter writer;
            opera.sendPlatformEvent(m_ui_view,writer.write(o));
            break;//"deleteBookmark"
        }
        case CMD_ID_GETALLBOOKMARKS:
        {
            Json::Value o;
            o["msg"] = getMessageStringValue(MESSAGE_GET_ALL_BOOKMARKS);
        #if 1 //def DATABASE_IMPLEMENT
            o["speeddials"] = m_bookmarkDB->getAllBookmark().c_str();
        #else
            o["speeddials"] = m_bookmarks.getAllBookmarkString();
        #endif
            Json::FastWriter writer;
            opera.sendPlatformEvent(m_ui_view, writer.write(o));
            break;//"getAllBookmarks"
        }
        case CMD_ID_GETHISTORY:
        {
            int top = getValueByName(event.message, "top", 0);
            Json::Value o;
            o["msg"] = getMessageStringValue(MESSAGE_GET_HISTORY);
        #if 1 //def DATABASE_IMPLEMENT
            if(top == 0){
                o["historys"] = m_historyDB->getAllHistory();
            }else{
                char topString[8] = {0};
                snprintf(topString,8,"%d",top);
                o["historys"] = m_historyDB->getTopHistory(std::string(topString));
            }
        #else
            o["historys"] = m_historys.getAllHistory();
        #endif
            Json::FastWriter writer;
            opera.sendPlatformEvent(m_ui_view, writer.write(o));
            break;//"getHistory"
        }
        case CMD_ID_CLEARHISTORY:
        {
            Json::Value o;
            o["msg"] = getMessageStringValue(MESSAGE_CLEAR_ALL_HISTORY);
            o["clearHistory"] = -1;
        #if 1 //def DATABASE_IMPLEMENT
            o["clearHistory"] = m_historyDB->clear();
        #else
            if (m_historys.clear() == 0) {
                saveHistory();
                o["clearHistory"] = 0;
            }
        #endif
            Json::FastWriter writer;
            opera.sendPlatformEvent(m_ui_view, writer.write(o));
            break;//"clearHistory"
        }
        case CMD_ID_CLEARCOOKIES:
        {
            opera.clearCookies();
            break;
        }
        case CMD_ID_GETSPEEDDIALS:
        {
            Json::Value o;
            o["msg"] = getMessageStringValue(MESSAGE_GET_SPEEDDIAL);
        #if 1 //def DATABASE_IMPLEMENT
            o["speeddials"] = m_speeddialDB->getAllBookmark();
        #else
            o["speeddials"] = m_speeddials.getAllBookmarkString();
        #endif
            Json::FastWriter writer;
            opera.sendPlatformEvent(m_ui_view,writer.write(o));
            break;//"getSpeedDials"
        }
        case CMD_ID_GETSPEEDDIALBYURL:
        {
            std::string url = getValueByName( event.message , "url" , "");
            Json::Value o;
            o["msg"] = getMessageStringValue(MESSAGE_GET_SPEEDDIAL_BY_URL);
        #if 1 //def DATABASE_IMPLEMENT
            o["speeddial"] = m_speeddialDB->getBookmarkByUrl(url);
        #else
            ///TODO:
        #endif
            Json::FastWriter writer;
            opera.sendPlatformEvent(m_ui_view,writer.write(o));
            break;
        }
        case CMD_ID_ADDSPEEDDIAL:
        {
            std::string url = getValueByName( event.message , "url" , "");
            std::string title = getValueByName( event.message , "title" , "");
            std::string thumbNail=getValueByName( event.message , "thumbNail" , "");
            int result;

            BROWSER_LOG(INFO, "CMD_ID_ADDSPEEDDAIL : url : %s, title : %s, thumbnail : %s",
                        url.c_str(), title.c_str(), thumbNail.c_str() );

        #if 1 //def DATABASE_IMPLEMENT
            result = m_speeddialDB->addBookmark(url,title,thumbNail);
            BROWSER_LOG(INFO, "CMD_ID_ADDSPEEDDAIL : result : %d", result);
        #else
            result = m_speeddials.addBookmark(url,title,thumbNail);
            saveSpeeddial();
        #endif

            Json::Value o;
            o["msg"] = getMessageStringValue(MESSAGE_ADD_SPEEDDIAL);
            o["url"] = url;
            o["title"] = title;
            o["result"] = result;
            Json::FastWriter writer;
            opera.sendPlatformEvent(m_ui_view,writer.write(o));
            break;//"addSpeedDial"
        }
        case CMD_ID_MOVESPEEDDIAL:
        {
            std::string url = getValueByName( event.message, "url", "");
            int position = getValueByName(event.message, "position", 0);
            bool result = false;

        #if 1 //def DATABASE_IMPLEMENT
            result = m_speeddialDB->moveBookmark(url, position);
        #else
            result = m_speeddials.moveBookmark(url, position);
            saveSpeeddial();
        #endif

            Json::Value o;
            o["msg"] = getMessageStringValue(MESSAGE_MOVE_SPEEDDIAL);
            o["url"] = url;
            o["result"] = result ? 0 : 1;
            Json::FastWriter writer;
            opera.sendPlatformEvent(m_ui_view,writer.write(o));
            break;//"moveSpeedDial"
        }
        case CMD_ID_REMOVESPEEDDIAL:
        {
            std::string url = getValueByName( event.message, "url", "");
            bool result = false;
        #if 1 //def DATABASE_IMPLEMENT
            result = m_speeddialDB->removeBookmark(url);
        #else
            result = m_speeddials.removeBookmark(url);
            saveSpeeddial();
        #endif
            Json::Value o;
            o["msg"] = getMessageStringValue(MESSAGE_REMOVE_SPEEDDIAL);
            o["url"] = url;
            o["result"] = result ? 0 : 1;
            Json::FastWriter writer;
            opera.sendPlatformEvent(m_ui_view,writer.write(o));
            break;//"removeSpeedDial"
        }
        case CMD_ID_MODIFY_SPEEDDIAL:
        {
            std::string url = getValueByName( event.message, "url", "");
            std::string newUrl = getValueByName( event.message, "newUrl", "");
            std::string newTitle = getValueByName( event.message, "newTitle", "");
            bool result = false;
        #if 1 //def DATABASE_IMPLEMENT
            result = m_speeddialDB->modifyBookmark(url, newUrl, newTitle);
        #else
            result = m_speeddials.modifyBookmark(url, newUrl, newTitle);
        #endif
            Json::Value o;
            o["msg"] ="modifySpeedDial";
            o["result"] = result ? 0 : 1;
            Json::FastWriter writer;
            opera.sendPlatformEvent(m_ui_view,writer.write(o));
            break;
        }
        case CMD_ID_DIALOGCLOSED:
            break;
        case CMD_ID_GETSETTINGS:
            break;//"getSettings"
        case CMD_ID_GETHIGHLIGHTEDURL:
            break;//"getHighlightedUrl"
        case CMD_ID_CHANGEPREFERENCES:
            break;
        case CMD_ID_ISFEATUREENABLED:
            break;//"isFeatureEnabled"

        /*added*/
        case CMD_ID_CAPTURE_THUMBNAIL:
            break;/*11*/
        case CMD_ID_THUMBNAIL_DATA:
        {
            std::string thumbnail = getValueByName( event.message , "thumbnail" ,"");
            std::string url = getValueByName( event.message, "url" ,"");
            std::string title = getValueByName( event.message, "title", "" );
            Json::Value o;
            o["msg"] = getMessageStringValue(MESSAGE_CREATE_THUMBNAIL);
            o["url"] = url;
            o["thumbnail"] = thumbnail;
            o["title"] = title;
            Json::FastWriter write;
            opera.sendPlatformEvent(m_ui_view , write.write(o));
        #if 1 //def DATABASE_IMPLEMENT
            m_speeddialDB->updateThumbnail(url, thumbnail);
            m_speeddialDB->updateTitle(url, title);
            m_bookmarkDB->updateThumbnail(url, thumbnail);
            m_bookmarkDB->updateTitle(url, title);
        #else
            m_speeddials.updateThumbnail(url, thumbnail);
            m_speeddials.updateTitle(url, title);
            m_bookmarks.updateThumbnail(url, thumbnail);
            m_bookmarks.updateTitle(url, title);
            saveBookmark();
            saveSpeeddial();
        #endif
            break;
        }
        case CMD_ID_WINDOWRESIZETO:
        {
            int x = getValueByName(event.message, "x", 0);
            int y = getValueByName(event.message, "y", 0);
            int width = getValueByName(event.message, "width", 1920);
            int height = getValueByName(event.message, "height", 1080);
            BROWSER_LOG( ALWAYS, "CMD_ID_WINDOWRESIZETO : x : %d, y : %d, width : %d, height : %d",
                        x, y, width, height );
            opera.setBounds(m_ui_view, x, y, width, height);
            Json::Value o;
            o["msg"] = "windowResizeTo";
            o["result"] = 0;
            Json::FastWriter writer;
            opera.sendPlatformEvent(m_ui_view, writer.write(o));
            break;
        }
        case CMD_ID_UPDATE_TITLE:
        {
            std::string url;
      //just recode history of main window
        if (event.window_id == main_window_vector.at(activeTabIndex).m_main_window) {
        url = getValueByName(event.message, "url", "");
        if (url.empty() || m_main_url == "undefined") {
          break;
        }
        main_window_vector.at(activeTabIndex).title = getValueByName(event.message, "title", "");
        std::string icon = getValueByName(event.message, "icon", "");

#ifdef DATABASE_IMPLEMENT
        m_historyDB->modifyTitle(url, main_window_vector.at(activeTabIndex).title);
#else
        m_historys.modifyTitle(url, main_window_vector.at(activeTabIndex).title);
        saveHistory();
#endif
      }
    break;
        }
/*
        case CMD_ID_UPDATE_ICON:
        {
            //just recode history of main window
            if(event.window_id == m_main_window)
            {
                m_main_url = getValueByName(event.message, "url", "");
                if ( m_main_url.empty() || m_main_url == "undefined" )
                {
                    break;
                }
                std::string icon = getValueByName(event.message, "icon", "");

            #if 1 //def DATABASE_IMPLEMENT
                m_historyDB->modifyICON(m_main_url, icon);
            #else
                m_historys.modifyICON(m_main_url, icon);
                saveHistory();
            #endif
            }
            break;
        }
        */
        case CMD_ID_DELETEHISTORY:
        {
            std::string url = getValueByName(event.message, "url", "");
            if ( url.empty() )
            {
                break;
            }
            else
            {
                std::string time = getValueByName(event.message, "time", "0");

            #if 1 //def DATABASE_IMPLEMENT
                m_historyDB->deleteSpecifiedHistory(url, time);
            #else
                m_historys.deleteSpecifiedHistory( url, time );
                saveHistory();
            #endif

                Json::Value o;
                o["msg"] = getMessageStringValue(MESSAGE_DELETE_HISTORY);
                o["result"] = 0;
                Json::FastWriter writer;
                opera.sendPlatformEvent(m_ui_view, writer.write(o));
            }
            break;
        }
        case CMD_ID_VIDEO_FULLSCREEN:
        {
            int state = getValueByName(event.message, "state", 0);
            if ( state == 1 )
            {
                BROWSER_LOG(INFO, "Enter video fullscreen mode");
                m_input_interceptor->setVideoFullscreen(true);
                opera.hideCursor(main_window_vector.at(activeTabIndex).m_main_window);
            }
            else
            {
                BROWSER_LOG(INFO, "Leave video fullscreen mode");
                m_input_interceptor->setVideoFullscreen(false);
            }
            Json::Value o;
            o["msg"] = getMessageStringValue(MESSAGE_VIDEO_FULLSCREEN_CHANGE);
            o["result"] = state;
            Json::FastWriter writer;
            opera.sendPlatformEvent(m_ui_view, writer.write(o));
            break;
        }
        case CMD_ID_VIDEO_PLAYED:
        {
            if (event.window_id == main_window_vector.at(activeTabIndex).m_main_window)
            {
                int x = getValueByName(event.message, "x", -1);
                int y = getValueByName(event.message, "y", -1);
                Json::Value o;
                o["msg"] = getMessageStringValue(MESSAGE_VIDEO_PLAYED);
                o["x"] = x;
                o["y"] = y;
                Json::FastWriter writer;
                opera.sendPlatformEvent(m_ui_view, writer.write(o));
            }
            break;
        }
        case CMD_ID_VIDEO_BACKWARD:
            if (event.window_id == m_ui_view){
                Json::Value o;
                o["msg"] = getMessageStringValue(MESSAGE_VIDEO_BACKWARD);
                Json::FastWriter writer;
                opera.sendPlatformEvent(main_window_vector.at(activeTabIndex).m_main_window, writer.write(o));
            }
            break;
        case CMD_ID_VIDEOPLAYPAUSE:
            if (event.window_id == m_ui_view){
                Json::Value o;
                o["msg"] = getMessageStringValue(MESSAGE_VIDEO_PLAYPAUSE);
                Json::FastWriter writer;
                opera.sendPlatformEvent(main_window_vector.at(activeTabIndex).m_main_window, writer.write(o));
            }
            break;
        case CMD_ID_VIDEOSTOP:
            if (event.window_id == m_ui_view){
                Json::Value o;
                o["msg"] = getMessageStringValue(MESSAGE_VIDEO_STOP);
                Json::FastWriter writer;
                opera.sendPlatformEvent(main_window_vector.at(activeTabIndex).m_main_window, writer.write(o));
            }
            break;
        case CMD_ID_VIDEOFORWARD:
            if (event.window_id == m_ui_view){
                Json::Value o;
                o["msg"] = getMessageStringValue(MESSAGE_VIDEO_FORWARD);
                Json::FastWriter writer;
                opera.sendPlatformEvent(main_window_vector.at(activeTabIndex).m_main_window, writer.write(o));
            }
            break;
        case CMD_ID_VIDEO_PAUSED:
            if (event.window_id == main_window_vector.at(activeTabIndex).m_main_window){
                Json::Value o;
                o["msg"] = getMessageStringValue(MESSAGE_VIDEO_PAUSED);
                Json::FastWriter writer;
                opera.sendPlatformEvent(m_ui_view, writer.write(o));
            }
            break;
       case CMD_ID_VIDEO_PLAYING:
            if (event.window_id == main_window_vector.at(activeTabIndex).m_main_window){
                Json::Value o;
                o["msg"] = getMessageStringValue(MESSAGE_VIDEO_PLAYING);
                Json::FastWriter writer;
                opera.sendPlatformEvent(m_ui_view, writer.write(o));
            }
            break;
        case CMD_ID_VIDEO_WAITING:
            if (event.window_id == main_window_vector.at(activeTabIndex).m_main_window){
                Json::Value o;
                o["msg"] = getMessageStringValue(MESSAGE_VIDEO_WAITING);
                Json::FastWriter writer;
                opera.sendPlatformEvent(m_ui_view, writer.write(o));
            }
            break;
        case CMD_ID_VIDEO_TIME_UPDATE:
            if (event.window_id == main_window_vector.at(activeTabIndex).m_main_window){
                Json::Value o;
                o["msg"] = getMessageStringValue(MESSAGE_VIDEO_TIME_UPDATE);
                std::string curTime = getValueByName( event.message, "curTime" ,"00:00:00");
                std::string totalTime = getValueByName( event.message, "totalTime", "00:00:00" );
                float progress = getValueByName( event.message, "progress", 0.0 );
                o["curTime"]=curTime;
                o["totalTime"]=totalTime;
                o["progress"]=progress;
                Json::FastWriter writer;
                opera.sendPlatformEvent(m_ui_view, writer.write(o));
            }
            break;
        case CMD_ID_VIDEO_ENDED:
            if (event.window_id == main_window_vector.at(activeTabIndex).m_main_window){
                Json::Value o;
                o["msg"] = getMessageStringValue(MESSAGE_VIDEO_ENDED);
                Json::FastWriter writer;
                opera.sendPlatformEvent(m_ui_view, writer.write(o));
            }
            break;
        case CMD_ID_VIDEO_SET_PERCENTAGE:
            if (event.window_id == m_ui_view) {
                Json::Value o;
                o["msg"] = getMessageStringValue(MESSAGE_VIDEO_SET_PERCENTAGE);
                int percentage = getValueByName( event.message, "percentage", 0 );
                o["percentage"] = percentage;

                Json::FastWriter writer;
                opera.sendPlatformEvent(main_window_vector.at(activeTabIndex).m_main_window, writer.write(o));
            }
            break;
        case CMD_ID_VIDEO_EXIT_FULLSCREEN:
            if (event.window_id == m_ui_view) {
                Json::Value o;
                o["msg"] = getMessageStringValue(MESSAGE_VIDEO_EXIT_FULLSCREEN);
                Json::FastWriter writer;
                opera.sendPlatformEvent(main_window_vector.at(activeTabIndex).m_main_window, writer.write(o));
            }
            break;
         case CMD_ID_GET_VIRTUAL_KEYBOARD_STATUS:
            if (event.window_id == m_ui_view) {
                Json::Value o;
                o["msg"] = getMessageStringValue(MESSAGE_GET_VIRTUAL_KEYBOARD_STATUS);
                o["isShow"]=browser().getVirtualKeyboard().isShow();
                Json::FastWriter writer;
                opera.sendPlatformEvent(m_ui_view,writer.write(o));
            }
            break;
        default:
            break;
    }
}
void BrowserModeHiBrowser::onUrlChange( opera::evt::URLChanged event)
{
    BROWSER_LOG(ALWAYS, "Url change window :%s url : %s", event.window_id.str().c_str(), event.url.c_str());
    HisenseOpera& opera = browser().getHisenseOpera();
  
    if((event.url.find("www.pga.com")!=std::string::npos)||
       (event.url.find("www.disney.com")!=std::string::npos)||
       (event.url.find("go.com")!=std::string::npos)||
       (event.url.find("hisense")!=std::string::npos)||
       (event.url.find("redbullracing.com")!=std::string::npos)||
       (event.url.find("www.mtv.com")!=std::string::npos)||
       (event.url.find("www.cbs.com")!=std::string::npos)||
       (event.url.find("www.si.com")!=std::string::npos)||
       (event.url.find("www.zynga.com")!=std::string::npos)||
       (event.url.find("www.elpais.com")!=std::string::npos)
       ||(event.url.find("wsj.com")!=std::string::npos)
       ||(event.url.find("allthingsd.com")!=std::string::npos)
       ||(event.url.find("yahoo.com")!=std::string::npos))
    {
        keyboard_search_flag = true;
    }else
    {
        keyboard_search_flag = false;
    }
    if((event.url.find("www.postmaster.co.uk")!=std::string::npos))
    {
        isSpecialURL = true;
    }
    else{
        isSpecialURL = false;
    }
    if((event.url.find("www.wsj.com")!=std::string::npos) ||
       (event.url.find("vod.walla.co.il")!=std::string::npos) ||
       ((event.url.find("ifeng.com")!=std::string::npos) && (event.url.find("v.ifeng.com")==std::string::npos) )||
       (event.url.find("msnbc.com")!=std::string::npos) ||
       (event.url.find("www.moviehd-master.com") != std::string::npos) ||
       (event.url.find("youtube.com")!=std::string::npos) ||
       (event.url.find("news.google") != std::string::npos) ||
       (event.url.find("www.bing") != std::string::npos) ||
       (event.url.find("web.de") != std::string::npos) ||
       (event.url.find("germanmail.com") != std::string::npos) ||
       (event.url.find("vera.com") != std::string::npos)||
       (event.url.find("yahoo.com")!=std::string::npos)||
       (event.url.find("www.altavista.com")!=std::string::npos)){
       if(!m_UAChangedToDefault){
          BROWSER_LOG(ALWAYS, "change UA to default");
          opera::Opera& opera_ = opera.getOpera();
          opera.setPref(  main_window_vector.at(activeTabIndex).m_main_window, "user_agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.71 Safari/537.36");
          opera_.post(opera::msg::LoadURL(event.window_id,event.url)).get();
          m_UAChangedToDefault=true;
       }
    }
    /*else if(event.url.find("about:blank")==std::string::npos){
       if(m_UAChangedToDefault){
          BROWSER_LOG(ALWAYS, "change UA to ipad");
          std::string uaString = browser().getConfiguration().getValueByKey( "BrowserUIUA" );
          opera.setPref( m_main_window, "user_agent", uaString );
          m_UAChangedToDefault=false;
       }
    }*/
    else if( (event.url.find("yahoo.com")!=std::string::npos)
        ||(event.url.find("facebook.com")!=std::string::npos))
    {
        if(!m_UAChangedToDefault){
            BROWSER_LOG(ALWAYS, "change UA to Android KitKat");
            opera.setPref(  main_window_vector.at(activeTabIndex).m_main_window, "user_agent", "Mozilla/5.0 (Linux; Android 4.4.2; Nexus 4 Build/KOT49H) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/34.0.1847.114 Mobile Safari/537.36"  );
            m_UAChangedToDefault=true;
        }
    }
    else if((event.url.find("bbc")!=std::string::npos))
    {
        if(!m_UAChangedToDefault){
            BROWSER_LOG(ALWAYS, "change UA to ipad");
            opera.setPref(  main_window_vector.at(activeTabIndex).m_main_window, "user_agent", "Mozilla/5.0 (iPad; CPU OS 8_1_3 like Mac OS X) AppleWebKit/600.1.4 (KHTML, like Gecko) Version/8.0 Mobile/12B466 Safari/600.1.4"  );
            m_UAChangedToDefault=true;
        }
    }
    else
    {
        BROWSER_LOG(ALWAYS, "change UA to freeBSD");
        opera.setPref(  main_window_vector.at(activeTabIndex).m_main_window, "user_agent", "Mozilla/5.0 (FreeBSD; Viera; ru:34.0) Gecko/20100101 Firefox/34.0");
        m_UAChangedToDefault=false;
    }
    if ( event.window_id ==  main_window_vector.at(activeTabIndex).m_main_window &&
        ( event.url.find( UNDEFINED_URL ) == std::string::npos ) ) {
        Json::Value o;
        o[ "msg" ] = getMessageStringValue(MESSAGE_HISTORY_CHANGED);
        o[ "url" ] = event.url;

        int index = opera.getCurrentHistoryIndex( main_window_vector.at(activeTabIndex).m_main_window);
        o[ "history_index" ] = index;
        int count = opera.getHistoryCount( main_window_vector.at(activeTabIndex).m_main_window);
        o[ "history_size" ] = count;
        //o[ "tab_id" ] = event.window_id.internal_id();
        o["tab_id"] = 0;
        Json::FastWriter write;
        opera.sendPlatformEvent( m_ui_view, write.write(o));
        if (event.url.find( BLANK_PAGE ) == std::string::npos)
        {
        #if 1 //def DATABASE_IMPLEMENT
            m_historyDB->addHistory(event.url, event.url, "");
        #else
            m_historys.addHistory(event.url, event.url, "");
            saveHistory();
        #endif
        }
    }
}

void BrowserModeHiBrowser::onNewWindow( opera::evt::NewWindow event)
{
}

void BrowserModeHiBrowser::onNewWindowBlocked( opera::evt::NewWindowBlocked event)
{
}

void BrowserModeHiBrowser::HandleRendererProcessDied(opera::evt::RendererProcessDied process_died)
{
    BROWSER_LOG(WARNING, "render process died window owner : %s", process_died.window_id.str().c_str());
    switch (process_died.status) {
        case opera::evt::RendererProcessDied::WAS_KILLED:
            BROWSER_LOG(WARNING, "The renderer process was killed");
            break;
        case opera::evt::RendererProcessDied::CRASHED:
            BROWSER_LOG(WARNING, "The renderer process crashed, because segmentation fault");
            break;
        default:
            BROWSER_LOG(WARNING, "The renderer process died");
            break;
    }
    if ( process_died.window_id == m_thumbnail_window )
    {
        HisenseOpera& opera = browser().getHisenseOpera();
        opera.destroyWindow(m_thumbnail_window);
        m_thumbnailWindowCreated=false;
        std::string uaString = browser().getConfiguration().getValueByKey( "BrowserUIUA" );
        m_thumbnail_window = opera.createWindow(false, true);
        opera.setPref( m_thumbnail_window, "content_resolution", THUMBNAIL_BOUNDS );
        opera.setPref( m_thumbnail_window, "user_agent", uaString );
    }
}

void BrowserModeHiBrowser::onLoadingProgress( opera::evt::LoadingProgress event)
{
    if (event.window_id.IsView())
        return;

    BROWSER_LOG(INFO, "Loading progress window :%s progress : %f", event.window_id.str().c_str(), event.progress);
    if( event.window_id == main_window_vector.at(activeTabIndex).m_main_window )
    {
        HisenseOpera& opera = browser().getHisenseOpera();
        Json::Value o;
        o["msg"] = getMessageStringValue(MESSAGE_LOADING_PROGRESS);
        o["progress"] = event.progress;
        Json::FastWriter write;

        //opera.sendPlatformEvent( m_main_window , write.write(o));
        opera.sendPlatformEvent( m_ui_view, write.write(o));
    }
    else
    {
        //do nothing
    }
}
void BrowserModeHiBrowser::setMouseStatus(bool shown){
    mouseShown=shown;
}
bool BrowserModeHiBrowser::getMouseStatus( ){
    return mouseShown;
}

#if 0 //!defined DATABASE_IMPLEMENT
std::string BrowserModeHiBrowser::getFileData(std::string fileName)
{
    BROWSER_LOG( INFO, "fileName : %s", fileName.c_str() );
    std::fstream fileStream;
    fileStream.open(fileName.c_str(), std::ios::in);

    std::string tempData;
    std::string finalData;

    //For " " separator
    while (fileStream >> tempData)
    {
        if (!finalData.empty())
        {
            finalData += " ";
        }
        finalData += tempData;
    }
    fileStream.close();
    return finalData;
}

void BrowserModeHiBrowser::saveToFile(std::string data, std::string fileName)
{
    BROWSER_LOG( INFO, "fileName : %s", fileName.c_str() );
    std::ofstream out(fileName.c_str());
    out.close();

    std::fstream fileStream;
    fileStream.open(fileName.c_str(), std::ios::out);
    fileStream << data;
    fileStream.close();
    fileStream.sync();
}

void BrowserModeHiBrowser::initBookmark()
{
    std::string fileData;
    fileData = getFileData(browser().getConfiguration().getValueByKey("Bookmark_File"));
    std::string maxSize;
    maxSize = browser().getConfiguration().getValueByKey("BookmarkMAXSize");
    int size = MAX_BOOKMARK_SIZE;
    if (!maxSize.empty())
    {
        size = atoi(maxSize.c_str());
    }
    m_bookmarks.init(fileData,size,BookmarkManager::BOOKMARK);
}

void BrowserModeHiBrowser::initSpeeddial()
{
    std::string fileData;
    fileData = getFileData(browser().getConfiguration().getValueByKey("SpeedDial_File"));
    std::string maxSize;
    maxSize = browser().getConfiguration().getValueByKey("SpeedDialMAXSize");
    int size = MAX_SPEEDIAL_SIZE;
    if (!maxSize.empty())
    {
        size = atoi(maxSize.c_str());
    }
    m_speeddials.init(fileData,size,BookmarkManager::SPEEDDIAL);
}


void BrowserModeHiBrowser::initHistory()
{
    std::string fileData;
    fileData = getFileData(browser().getConfiguration().getValueByKey("History_File"));
    std::string maxSize;
    maxSize = browser().getConfiguration().getValueByKey("HistoryMAXSize");
    int size = 0;
    if (!maxSize.empty())
    {
        size = atoi(maxSize.c_str());
    }
    m_historys.init(fileData, size);
}
#endif

void BrowserModeHiBrowser::initCurrentWindowData()
{
    m_currentWindowInfo.title = "";
    m_currentWindowInfo.url = "";
    m_currentWindowInfo.zoomFactor = 1;
}

#if 0 //!defined DATABASE_IMPLEMENT
void BrowserModeHiBrowser::saveSpeeddial()
{
    saveToFile(m_speeddials.getAllBookmarkString(),browser().getConfiguration().getValueByKey("SpeedDial_File"));
}

void BrowserModeHiBrowser::saveBookmark()
{
    saveToFile(m_bookmarks.getAllBookmarkString(),browser().getConfiguration().getValueByKey("Bookmark_File"));
}

void BrowserModeHiBrowser::saveHistory()
{
    saveToFile(m_historys.getAllHistory(), browser().getConfiguration().getValueByKey("History_File"));
}
#endif

std::string BrowserModeHiBrowser::getValueByName(std::string msg,std::string name,const char* defaultValue)
{
   return getValueByName(msg,name,(std::string)defaultValue);
}

std::string BrowserModeHiBrowser::getValueByName(std::string msg,std::string name,const std::string defaultValue)
{
    std::string result(defaultValue);
    Json::Reader reader;
    Json::Value  value;
    bool parse_result = reader.parse(msg, value);
    if(parse_result)
    {
        if ( value.get(name, defaultValue).isString() )
        {
            result = value.get(name, defaultValue).asString();
        }
    }
    return result;
}

int BrowserModeHiBrowser::getValueByName(std::string msg,std::string name,int defaultValue)
{
    int result(defaultValue);
    Json::Reader reader;
    Json::Value  value;
    bool parse_result = reader.parse(msg, value);
    if(parse_result)
    {
        if ( value.get(name, defaultValue).isInt() )
        {
            result = value.get(name, defaultValue).asInt();
        }
    }
    return result;
}

bool BrowserModeHiBrowser::getValueByName(std::string msg,std::string name,bool defaultValue)
{
    bool result(defaultValue);
    Json::Reader reader;
    Json::Value  value;
    bool parse_result = reader.parse(msg, value);
    if(parse_result)
    {
        if ( value.get(name, defaultValue).isBool() )
        {
            result = value.get(name, defaultValue).asBool();
        }
    }
    return result;
}

float BrowserModeHiBrowser::getValueByName(std::string msg,std::string name, double defaultValue)
{
    float result(defaultValue);
    Json::Reader reader;
    Json::Value  value;
    bool parse_result = reader.parse(msg, value);
    if(parse_result)
    {
        if ( value.get(name, defaultValue).isDouble() )
        {
            result = value.get(name, defaultValue).asDouble();
        }
    }
    return result;
}

BrowserModeHiBrowser::Type BrowserModeHiBrowser::getCmdType(std::string msg)
{
    BrowserModeHiBrowser::Type result(CMD_ID_INVALID);
    //BROWSER_LOG(DETAIL, "");
    Json::Reader reader;
    Json::Value  value;
    bool parse_result = reader.parse(msg, value);
    if(parse_result)
    {
        return (BrowserModeHiBrowser::Type)value.get("type",CMD_ID_INVALID).asInt();
    }
    return result;
}
