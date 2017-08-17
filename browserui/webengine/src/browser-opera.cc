#include <json/json.h>

#include <opera/Opera.h>
#include <opera/messages/CreateTVStoreWindow.h>
#include <opera/messages/CreateBrowserUIWindow.h>
#include <opera/messages/CreateWindow.h>
#include <opera/messages/DestroyWindow.h>
#include <opera/messages/Show.h>
#include <opera/messages/Hide.h>
#include <opera/messages/Focus.h>
#include <opera/messages/LoadURL.h>
#include <opera/messages/PageZoom.h>
#include <opera/messages/Blur.h>
#include <opera/messages/ParseAIT.h>
#include <opera/messages/InvalidateAIT.h>
#include <opera/messages/TerminateApplication.h>
#include <opera/messages/SetPref.h>
#include <opera/messages/GetPref.h>
#include <opera/messages/MoveInHistory.h>
#include <opera/messages/KeyInput.h>
#include <opera/messages/CommitIME.h>
#include <opera/messages/CancelIME.h>
//#include <opera/messages/CreateHTMLView.h>
//#include <opera/messages/DestroyHTMLView.h>
#include <opera/messages/StopLoading.h>
#include <opera/messages/ClearApplicationCache.h>
#include <opera/messages/ClearCookies.h>
#include <opera/messages/ClearDiskCache.h>
#include <opera/messages/ClearLocalStorage.h>
#include <opera/messages/ExitFullscreenMode.h>
#include <opera/messages/SendPlatformEvent.h>
#include <opera/messages/GetWindowMemoryInformation.h>
#include <opera/messages/GetBrowserMemoryInformation.h>
#include <opera/messages/InjectRemoteXmlAIT.h>
#include <opera/messages/LoadBroadcastIndependentApplication.h>
#include <opera/messages/LoadBroadcastRelatedApplication.h>
#include <opera/messages/RaiseDSMCCStreamEvent.h>
//#include <opera/messages/SetHTMLViewOpacity.h>
#include <opera/messages/SetTimeShiftMode.h>
#include <opera/messages/Reload.h>
#include <opera/messages/SetBounds.h>
#include <opera/messages/StackAtTop.h>
#include <opera/messages/StackAtBottom.h>
#include <opera/messages/StackAbove.h>
#include <opera/messages/StackBelow.h>
#include <opera/messages/CreateOIPFApplication.h>
#include <opera/messages/CreateOIPFWindow.h>
#include <opera/messages/ActivateOIPFApplication.h>
#include <opera/messages/GetCurrentHistoryIndex.h>
#include <opera/messages/GetHistoryCount.h>
#include <opera/messages/ShowCursor.h>
#include <opera/messages/HideCursor.h>
#include <opera/messages/GetLastScrollOffset.h>
#include <opera/messages/GetMaxScrollOffset.h>
#include <opera/messages/ScrollTo.h>

#include "browser-opera.h"
#include "configuration.h"
#include "graphics.h"
#include "browser-impl.h"
#include "browser_debug.h"
#include "async_browser_task.h"
#include "task_manager.h"
#include "systeminfo.h"
#include "NotImplemented.h"
#define TWOK_WIDTH 1920
#define TWOK_HEIGHT 1080
#define StopCurrentMode 1

const std::string oipf_ua_suffix = "(Hisense;LTDN55K321UWT;F0625) CE-HTML/1.0 NETRANGEMMH HbbTV/1.2.1 MTK5657SA";

HisenseOpera::HisenseOpera( BrowserImpl* browserImpl )
    : BrowserComponent( browserImpl )
    , m_operaController( NULL )
{
    BROWSER_LOG( INFO, "" );
}

HisenseOpera::~HisenseOpera()
{
    // TODO(sunkai5): It gets here after loading AIT.
    // stop();
    // ///TODO:How to delete the release the Opera and OperaContorller
    //
    // if ( m_operaController ) {
    //     delete m_operaController;
    //     m_operaController = NULL;
    // }
    BROWSER_LOG( INFO, "" );
}

void HisenseOpera::GenerateDeviceID()
{
    BROWSER_LOG( INFO, "" );

    // it will be used by mtk drm, oipf and browser storage encryption
    int result = GenerateDeviceId();
    if (0 != result)
    {
        BROWSER_LOG( ERROR, "GenerateDeviceId failed, result: %d", result );
    }
}

void HisenseOpera::SetStoragePassword()
{
    BROWSER_LOG( INFO, "" );

    char device_guid[GUID_LENGTH + 1]{0};
    int result = GetDeviceGuid(device_guid, GUID_LENGTH + 1);
    if (0 != result)
    {
        BROWSER_LOG( ERROR, "GetDeviceGuid failed, result: %d", result );
        return;
    }

    opera::StaticConfiguration& opera_static_configuration = browser().getGraphics().getOperaConfiguration();
    opera_static_configuration.storage_password = device_guid;

    BROWSER_LOG( INFO, "storage password: %s", opera_static_configuration.storage_password.c_str() );
}

void HisenseOpera::SetTVStoreDeviceModelName()
{
    BROWSER_LOG( INFO, "" );

    std::string device_model_name = browser().getConfiguration().getValueByKey( "OperaTVStoreUA" );
    if (device_model_name.empty()) {
        device_model_name = "Demo-Blink";
    }

    opera::StaticConfiguration& opera_static_configuration = browser().getGraphics().getOperaConfiguration();
    opera_static_configuration.device_model_name = device_model_name;

    BROWSER_LOG( INFO, "device model name for TV Store: %s", opera_static_configuration.device_model_name.c_str() );
}

BrowserStatus HisenseOpera::initialize()
{
    GenerateDeviceID();

    SetStoragePassword();

    SetTVStoreDeviceModelName();

    Configuration& configuration = browser().getConfiguration();

    m_opera.reset(new opera::Opera(configuration.getCommandLine().argc(),
                               configuration.getCommandLine().argv(),
                               browser().getGraphics().getOperaConfiguration()));

    m_operaController = new opera::components::OperaController(*m_opera);
    if ( !m_operaController ) {
        BROWSER_LOG( ERROR, "Failed to construct OperaController");
        return BrowserError;
    }

    return BrowserOK;
}


BrowserStatus HisenseOpera::start()
{
    std::string userAgent = browser().getConfiguration().getValueByKey( "OperaTVStoreUA" );
    m_operaStoreUA = userAgent.empty() ? "Model/Demo-Blink" : userAgent;
    BROWSER_LOG(INFO, "Opera Store UserAgent : %s", m_operaStoreUA.c_str() );

    std::string statusPage = browser().getConfiguration().getValueByKey( "StoreStatusPage" );
    ///TODO:Default status page dir
    m_operaStatusPage = statusPage.empty() ? "/3rd_rw/statusWindow.html" : statusPage;
    BROWSER_LOG(INFO, "Loading status page : %s", m_operaStatusPage.c_str() );

    std::string operaStoreUrl = browser().getConfiguration().getValueByKey( "OperaTVStoreUrl" );
    m_operaStoreDefaultUrl = operaStoreUrl.empty() ? "https://demo.tvstore.opera.com/" : operaStoreUrl;
    BROWSER_LOG(INFO, "Opera Store default url : %s", operaStoreUrl.c_str() );

    std::string netrange_url = browser().getConfiguration().getValueByKey( "NetrangeUrl" );
    m_netrange_url = netrange_url.empty() ?
        "http://670ec95586bf3a7229d89377566498fd.b406e00761a0902f2c52bad12b6022c7.com/" : netrange_url;
    BROWSER_LOG(INFO, "Netrange default portal url : %s", m_netrange_url.c_str() );

    std::string foxxum_url = browser().getConfiguration().getValueByKey( "FoxxumUrl" );
    m_foxxum_url = foxxum_url.empty() ?
        "http://9706459157.fxm3751860294.com/index.php/appstore.html?platform=5657" : foxxum_url;
    BROWSER_LOG(INFO, "Foxxum default portal url : %s", m_foxxum_url.c_str() );

    std::string error_page = browser().getConfiguration().getValueByKey( "ErrorPage" );
    m_error_page = error_page.empty() ?
        "/3rd/opera4x/opera_dir/errorPage/network_error.html" : error_page;
    BROWSER_LOG(INFO, "error page url : %s", m_error_page.c_str() );

    std::string browerUIUserAgent = browser().getConfiguration().getValueByKey( "BrowserUIUA" );
    m_browerUIUserAgent = browerUIUserAgent.empty() ?
        "Mozilla/5.0 (iPad; CPU OS 5_1 like Mac OS X; en-us) AppleWebKit/534.46 (KHTML, like Gecko) Version/5.1 Mobile/9B176 Safari/7534.48.3"
        : browerUIUserAgent;
    BROWSER_LOG(INFO, "Browser UI user agent : %s", m_browerUIUserAgent.c_str() );

    //Always listen memory event of browser
    AddStationaryEventHandler( &HisenseOpera::browserMemoryEvent, this );
    AddStationaryEventHandler( &HisenseOpera::onStopPlatformMessage, this );
    //AddStationaryEventHandler( &HisenseOpera::ScriptRequestCloseWindow, this );

    return (m_operaController->Run()) ? BrowserOK : BrowserError;
}

void HisenseOpera::stop()
{
    BROWSER_LOG( INFO, "" );
    m_operaController->Stop();
}

opera::Handle HisenseOpera::createStoreWindow(const BrowserStoreWindowType& type, const std::string& url)
{
    BROWSER_LOG( ALWAYS, "BrowserStoreType : %d, url : %s", type, url.c_str() );
    opera::Handle window_id;
    switch( type ) {
        case OPERA_TV_STORE_WINDOW:
        {
            bool visible = false;
            if(url.find("tvstore.opera.com:84/api/tvapps/runapp") == std::string::npos)
            {
                visible = true;
            }
            #ifdef OPERASDK4_6
            window_id = m_opera->post( opera::msg::CreateTVStoreWindow(
                url.empty() ? m_operaStoreDefaultUrl : url, m_operaStoreUA ,visible ) ).get().window_id;
            #else
            window_id = m_opera->post( opera::msg::CreateTVStoreWindow(
                url.empty() ? m_operaStoreDefaultUrl : url,  visible ) ).get().window_id;
            #endif
            ///TODO:virtual keyboard for tv store?
            //setInputMethod( window_id, opera::msg::SetInputMethod::VIRTUAL_KEYBOARD );
            setInputMethod( window_id, opera::msg::SetInputMethod::OMI_EVENTS_INPUT_METHOD );

            break;
        }
        case OIPF_APP_STORE_WINDOW:
        {
             window_id = m_opera->post( opera::msg::CreateOIPFWindow( oipf_ua_suffix
                ) ).get().window_id;

            ///TODO:virtual keyboard for tv store?
            //setInputMethod( window_id, opera::msg::SetInputMethod::VIRTUAL_KEYBOARD );
            setInputMethod( window_id, opera::msg::SetInputMethod::OMI_EVENTS_INPUT_METHOD );
            break;
        }
        case OTHER_STORE_WINDOW:
        default:
        {
            window_id = m_opera->post( opera::msg::CreateWindow(false,false) ).get().window_id;
            m_opera->post(opera::msg::SetPref(window_id,"transparent","true")).get();

            if ( !url.empty() ) {
                opera::msg::LoadURL::Result result = m_opera->post( opera::msg::LoadURL( window_id, url ) ).get();
                if ( result.error != opera::msg::LoadURL::NO_ERR )
                    BROWSER_LOG( ERROR, "Failed to Load URL, error : %s", result.errorString().c_str() );
            }

            ///TODO:virtual keyboard for tv store?
            setInputMethod( window_id, opera::msg::SetInputMethod::OMI_EVENTS_INPUT_METHOD );
            break;
        }
    }

#if 0
    int width = browser().getGraphics().getWidth();
    int height = browser().getGraphics().getHeight();

    if ( width == TWOK_WIDTH && height == TWOK_HEIGHT ) {
        // The Page zoom message don't support setting zoom before loading finished
        setPref( window_id, "content_resolution", HD );
        //setPageZoom( window_id, 1.5);
    }
#endif

    BROWSER_LOG(INFO, "return window_id: %s", window_id.str().c_str() );
    return window_id;
}



opera::Handle HisenseOpera::createHBBTVWindow(const std::string ua_extra_capabilities,
                                              const std::string ua_vendor_name,
                                              const std::string ua_model_name,
                                              const std::string ua_software_version,
                                              const std::string ua_hardware_version,
                                              const std::string ua_family_name,
                                              unsigned short application_profile,
                                              std::vector<opera::msg::CreateHbbTVWindow::PlatformProfile> platform_profiles,
                                              int key_set_mask)
{
    BROWSER_LOG( INFO, "ua_extra_capabilities : %s", ua_extra_capabilities.c_str() );
    BROWSER_LOG( INFO, "ua_vendor_name : %s", ua_vendor_name.c_str() );
    BROWSER_LOG( INFO, "ua_model_name : %s", ua_model_name.c_str() );
    BROWSER_LOG( INFO, "ua_software_version : %s", ua_software_version.c_str() );
    BROWSER_LOG( INFO, "ua_hardware_version : %s", ua_hardware_version.c_str() );
    BROWSER_LOG( INFO, "ua_family_name : %s", ua_family_name.c_str() );
    BROWSER_LOG( INFO, "application_profile : %d", application_profile );
    BROWSER_LOG( INFO, "key_set_mask : %d", key_set_mask );

    opera::msg::CreateHbbTVWindow create_hbbtv_window;

    create_hbbtv_window.ua_extra_capabilities = ua_extra_capabilities;
    create_hbbtv_window.ua_vendor_name = ua_vendor_name;
    create_hbbtv_window.ua_model_name = ua_model_name;
    create_hbbtv_window.ua_software_version = ua_software_version;
    create_hbbtv_window.ua_hardware_version = ua_hardware_version;
    create_hbbtv_window.ua_family_name = ua_family_name;
    create_hbbtv_window.application_profile = (opera::msg::CreateHbbTVWindow::ApplicationProfile)(application_profile);
    create_hbbtv_window.platform_profiles = platform_profiles;
    create_hbbtv_window.key_set_mask = key_set_mask;

    BROWSER_LOG( INFO, "result application_profile : %d", create_hbbtv_window.application_profile );

    opera::Handle window_id = m_opera->post( create_hbbtv_window ).get().window_id;

    // the preference "content_resolution" of HBBTV window is read only,  that is the content_resolution is 1280x720
#if 0
    int width = browser().getGraphics().getWidth();
    int height = browser().getGraphics().getHeight();

    if ( width == TWOK_WIDTH && height == TWOK_HEIGHT ) {
        // The Page zoom message don't support setting zoom before loading finished
        setPref( window_id, "content_resolution", "1280x720");
        //setPageZoom( window_id, 1.5);
    }
#endif

    // HBBTV window don't support create HTML view

    // TODO(sunkai5): virtual keyboard for hbbtv?
    setInputMethod( window_id, opera::msg::SetInputMethod::VIRTUAL_KEYBOARD );
    // setInputMethod( window_id, opera::msg::SetInputMethod::OMI_EVENTS_INPUT_METHOD );

    BROWSER_LOG(INFO, "return window_id: %s", window_id.str().c_str() );
    return window_id;
}


opera::Handle HisenseOpera::createStatusWindow(const opera::Handle& group_with)
{
    BROWSER_LOG( INFO, "");

    std::string statusWindowWidth = browser().getConfiguration().getValueByKey("1080PStatusWindowWidth");
    std::string StatusWindowHeight = browser().getConfiguration().getValueByKey("1080PStatusWindowHeight");

    int statusWidth = 0;
    int statusHeight = 0;
    if (!statusWindowWidth.empty())
        statusWidth = atoi(statusWindowWidth.c_str());
    if (!StatusWindowHeight.empty())
        statusHeight = atoi(StatusWindowHeight.c_str());

    int browserWidth = browser().getGraphics().getWidth();
    int browserHeight = browser().getGraphics().getHeight();

#if 0 //def WINDOW_TRANS
    #ifdef OPERASDK4_6
        opera::Handle window_id = m_opera->post( opera::msg::CreateWindow( true,
            false, false, opera::Rect(( browserWidth - statusWidth ) / 2,
                                      ( browserHeight - statusHeight ) / 2,
                                      statusWidth, statusHeight), group_with ) ).get().window_id;
    #else
         opera::Handle window_id = m_opera->post( opera::msg::CreateWindow(
            false, false, opera::Rect(( browserWidth - statusWidth ) / 2,
                                      ( browserHeight - statusHeight ) / 2,
                                      statusWidth, statusHeight), group_with ) ).get().window_id;
    #endif
#else
    opera::Handle window_id = m_opera->post( opera::msg::CreateWindow(
        false, false, opera::Rect(( browserWidth - statusWidth ) / 2,
                                  ( browserHeight - statusHeight ) / 2,
                                  statusWidth, statusHeight), group_with ) ).get().window_id;
#endif
    BROWSER_LOG(INFO, "return window_id: %s", window_id.str().c_str() );

    std::string statusPage = m_operaStatusPage + "?resolution=1080";
    BROWSER_LOG(INFO, "statusPage: %s", statusPage.c_str() );
    opera::msg::LoadURL::Result result = m_opera->post( opera::msg::LoadURL( window_id, statusPage ) ).get();
    m_opera->post(opera::msg::SetPref(window_id,"transparent","true")).get();
    m_opera->post(opera::msg::Hide(window_id)).get();
    if ( result.error != opera::msg::LoadURL::NO_ERR ){
        BROWSER_LOG( ERROR, "Failed to Load URL, error : %s", result.errorString().c_str() );
    }
    m_opera->post(opera::msg::SetBounds(window_id,opera::Rect(( browserWidth - statusWidth ) / 2,
                                                      ( browserHeight - statusHeight ) / 2,statusWidth,statusHeight)));

    return window_id;
}
//
//opera::Handle HisenseOpera::createStatusHTMLView( const opera::Handle window )
//{
//    BROWSER_LOG( INFO, "");
//
//    std::string statusWindowWidth = browser().getConfiguration().getValueByKey("720PStatusWindowWidth");
//    std::string StatusWindowHeight = browser().getConfiguration().getValueByKey("720PStatusWindowHeight");
//
//    int statusWidth;
//    int statusHeight;
//    if (!statusWindowWidth.empty())
//        statusWidth = atoi(statusWindowWidth.c_str());
//    if (!StatusWindowHeight.empty())
//        statusHeight = atoi(StatusWindowHeight.c_str());
//
//    int browserWidth = 1280;//HTMLView status for store, store is 720P
//    int browserHeight = 720;//HTMLView status for store, store is 720P
//
//    std::string statusPage = m_operaStatusPage + "?resolution=720";
//    BROWSER_LOG( INFO, "statusPage: %s", statusPage.c_str() );
//    BROWSER_LOG( INFO, "statusWidth : %d, statusHeight : %d, browserWidth : %d, browserHeight : %d",
//                statusWidth, statusHeight, browserWidth, browserHeight );
//    opera::Handle html_view = m_opera->post( opera::msg::CreateHTMLView(
//        window, statusPage, opera::Rect(( browserWidth - statusWidth ) / 2,
//                                  ( browserHeight - statusHeight ) / 2,
//                                  statusWidth, statusHeight) ) ).get().view_id;
//    BROWSER_LOG(INFO, "return view_id: %s", html_view.str().c_str() );
//
//    return html_view;
//}

opera::Handle HisenseOpera::createBrowserUIWindow( const std::string& startUrl )
{
    BROWSER_LOG( INFO, "startUrl : %s", startUrl.c_str() );
    opera::msg::CreateBrowserUIWindow::Result result;
    if ( startUrl.empty() )
        result = m_opera->post(opera::msg::CreateBrowserUIWindow()).get();
    else
        result = m_opera->post(opera::msg::CreateBrowserUIWindow( startUrl )).get();

    if (result.error == opera::msg::CreateBrowserUIWindow::NOT_SUPPORTED)
        BROWSER_LOG( ERROR, "Failed to create BrowserUI window" );

    BROWSER_LOG(INFO, "return window_id: %s", result.window_id.str().c_str() );
    setPref( result.window_id, "user_agent", m_browerUIUserAgent );

    ///TODO:Temp to set opera virtual keyboard to BrowerUI
    //setInputMethod( result.window_id, opera::msg::SetInputMethod::VIRTUAL_KEYBOARD );
    setInputMethod( result.window_id, opera::msg::SetInputMethod::OMI_EVENTS_INPUT_METHOD );

    return result.window_id;
}
//
//opera::Handle HisenseOpera::createHTMLView( const opera::Handle& window, const std::string& url,
//                                                 int x, int y, int width, int height,
//                                                 bool visible )
//{
//    BROWSER_LOG( INFO, "create html view (url : %s) by window : %s, ", url.c_str(), window.str().c_str() );
//    opera::msg::CreateHTMLView::Result result = m_opera->post(
//        opera::msg::CreateHTMLView( window, url, opera::Rect( x, y, width, height ), visible ) ).get();
//    if (result.error != opera::msg::CreateHTMLView::NO_ERR)
//        BROWSER_LOG( ERROR, "Failed to create HTML View, error : %s", result.errorString().c_str() );
//
//    BROWSER_LOG(INFO, "return view_id: %s", result.view_id.str().c_str() );
//
//    return result.view_id;
//}

opera::Handle HisenseOpera::createWindow(
#if 0 //def WINDOW_TRANS
                                              bool transparent,
#endif
                                              bool visible, bool focused,
                                              const BrowserRect_t& rect, const opera::Handle& group_with,
                                              int priority, bool private_browsing, const std::string& browser_context)
{
#if 0 //def WINDOW_TRANS
    BROWSER_LOG( INFO, "visible : %d, transparent : %d", visible, transparent );
    #ifdef OPERASDK4_6
            opera::msg::CreateWindow::Result result = m_opera->post( opera::msg::CreateWindow(transparent, visible, focused ) ).get();
    #else
            opera::msg::CreateWindow::Result result = m_opera->post( opera::msg::CreateWindow( visible, focused ) ).get();
    #endif
#else
    BROWSER_LOG( INFO, "visible : %d", visible);
    opera::msg::CreateWindow::Result result = m_opera->post( opera::msg::CreateWindow( visible, focused,
                                                                                       opera::Rect( rect.x, rect.y, rect.width, rect.height ),
                                                                                       group_with, priority, private_browsing, browser_context) ).get();
#endif
    BROWSER_LOG(INFO, "window_id: %s", result.window_id.str().c_str() );
    return result.window_id;
}

void HisenseOpera::createOIPFApplication( opera::Handle& window, std::string app_url)
{
    BROWSER_LOG( INFO, "window : %s, app_url : %s", window.str().c_str(), app_url.c_str() );
    opera::msg::CreateOIPFApplication::Result result = m_opera->post( opera::msg::CreateOIPFApplication( window, 0, app_url ) ).get();
    if ( result.error != opera::msg::CreateOIPFApplication::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to create OIPF application, error : %s", result.errorString().c_str() );
}


void HisenseOpera::show( const opera::Handle& window )
{
    BROWSER_LOG( INFO, "show Handle : %s", window.str().c_str() );
    opera::msg::Show::Result result = m_opera->post( opera::msg::Show( window ) ).get();
    if ( result.error != opera::msg::Show::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to Show the window : %s, error : %s", window.str().c_str(), result.errorString().c_str() );
}

void HisenseOpera::hide( const opera::Handle& window )
{
    BROWSER_LOG( INFO, "hide Handle : %s", window.str().c_str() );
    opera::msg::Hide::Result result = m_opera->post( opera::msg::Hide( window ) ).get();
    if ( result.error != opera::msg::Hide::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to Hide the window : %s, error : %s", window.str().c_str(), result.errorString().c_str() );
}
void HisenseOpera::focus( const opera::Handle& window )
{
    BROWSER_LOG( INFO, "focus Handle : %s", window.str().c_str() );
    opera::msg::Focus::Result result = m_opera->post( opera::msg::Focus( window ) ).get();
    if ( result.error != opera::msg::Focus::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to Focus the window : %s, error : %s", window.str().c_str(), result.errorString().c_str() );
}
void HisenseOpera::blur()
{
    BROWSER_LOG( INFO, "" );
    m_opera->post( opera::msg::Blur() );
}

BrowserStatus HisenseOpera::setPageZoom( const opera::Handle& window, double factor )
{
    BrowserStatus zoomResult = BrowserOK;
    BROWSER_LOG( INFO, "Set zoom Handle : %s, factor : %f", window.str().c_str(), factor );
    opera::msg::PageZoom::Result result = m_opera->post( opera::msg::PageZoom( window, factor ) ).get();
    if ( result.error != opera::msg::PageZoom::NO_ERR ) {
        zoomResult = BrowserError;
        BROWSER_LOG( ERROR, "Failed to set zoom window : %s, error : %s", window.str().c_str(), result.errorString().c_str() );
    }

    return zoomResult;
}

void HisenseOpera::getCurrentScroll( const opera::Handle& window, int& x, int& y )
{
    BROWSER_LOG( INFO, "get current Scroll Handle : %s", window.str().c_str() );
    opera::msg::GetLastScrollOffset::Result result = m_opera->post( opera::msg::GetLastScrollOffset( window ) ).get();
    if ( result.error != opera::msg::GetLastScrollOffset::NO_ERR ) {
        x = -1;
        y = -1;
        BROWSER_LOG( ERROR, "Failed to get current scroll window : %s, error : %s", window.str().c_str(), result.errorString().c_str() );
    } else {
        x = result.x;
        y = result.y;
    }
}

void HisenseOpera::getMaxScroll( const opera::Handle& window, int& x, int& y )
{
   //BROWSER_LOG( INFO, "get MAX Scroll Handle : %s", window.str().c_str() );
    opera::msg::GetMaxScrollOffset::Result result = m_opera->post( opera::msg::GetMaxScrollOffset( window ) ).get();
    if ( result.error != opera::msg::GetMaxScrollOffset::NO_ERR ) {
        x = -1;
        y = -1;
        BROWSER_LOG( ERROR, "Failed to get MAX scroll window : %s, error : %s", window.str().c_str(), result.errorString().c_str() );
    } else {
        x = result.x;
        y = result.y;
    }
}

void HisenseOpera::setScroll( const opera::Handle& window, int x, int y )
{
    BROWSER_LOG( INFO, "set Scroll Handle : %s, x : %d, y : %d", window.str().c_str(), x, y );
    opera::msg::ScrollTo::Result result = m_opera->post( opera::msg::ScrollTo( window, x, y ) ).get();
    if ( result.error != opera::msg::ScrollTo::NO_ERR ) {
       BROWSER_LOG( ERROR, "Failed to set scroll window : %s, error : %s", window.str().c_str(), result.errorString().c_str() );
    }
}

void HisenseOpera::stopLoading( const opera::Handle& window )
{
    BROWSER_LOG( INFO, "stop loading  Handle : %s", window.str().c_str() );
    opera::msg::StopLoading::Result result = m_opera->post( opera::msg::StopLoading( window ) ).get();
    if ( result.error != opera::msg::StopLoading::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to destroy the window : %s, error : %s", window.str().c_str(), result.errorString().c_str() );
}

void HisenseOpera::destroyWindow( const opera::Handle& window )
{
    BROWSER_LOG( INFO, "destroy Handle : %s", window.str().c_str() );
    opera::msg::DestroyWindow::Result result = m_opera->post( opera::msg::DestroyWindow( window ) ).get();
    if ( result.error != opera::msg::DestroyWindow::NO_ERR ){
        BROWSER_LOG( ERROR, "Failed to destroy the window : %s, error : %s", window.str().c_str(), result.errorString().c_str() );
    }

}

//void HisenseOpera::destroyHTMLView( const opera::Handle& view )
//{
//    BROWSER_LOG( INFO, "destroy Handle : %s", view.str().c_str() );
//    opera::msg::DestroyHTMLView::Result result = m_opera->post( opera::msg::DestroyHTMLView( view ) ).get();
//    if ( result.error != opera::msg::DestroyHTMLView::NO_ERR )
//        BROWSER_LOG( ERROR, "Failed to destroy the view : %s, error : %s", view.str().c_str(), result.errorString().c_str() );
//}

void HisenseOpera::loadURL( const opera::Handle& window, const std::string& url )
{
    BROWSER_LOG( INFO, "Handle : %s, url : %s", window.str().c_str(), url.c_str() );
    opera::msg::LoadURL::Result result = m_opera->post( opera::msg::LoadURL( window, (url.empty()) ? "about::blank" : url ) ).get();
    if ( result.error != opera::msg::LoadURL::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to Load URL, error : %s", result.errorString().c_str() );
}

void HisenseOpera::setPref( const opera::Handle& window, const std::string& key, const std::string& value )
{
    BROWSER_LOG( INFO, "Handle : %s, key : %s, value : %s", window.str().c_str(), key.c_str(), value.c_str() );
    opera::msg::SetPref::Result result = m_opera->post(opera::msg::SetPref(window, key, value)).get();
    if ( result.error != opera::msg::SetPref::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to set preference window : %s, error : %s", window.str().c_str(), result.errorString().c_str() );
}

std::string HisenseOpera::getPref( const opera::Handle& window, const std::string& key )
{
    BROWSER_LOG( INFO, "Handle : %s, key : %s", window.str().c_str(), key.c_str() );
    opera::msg::GetPref::Result result = m_opera->post(opera::msg::GetPref(window, key)).get();
    if ( result.error != opera::msg::GetPref::NO_ERR ) {
        BROWSER_LOG( ERROR, "Failed to get preference window : %s, error : %s", window.str().c_str(), result.errorString().c_str() );
        return "ERROR";
    } else {
        return result.value;
    }
}

void HisenseOpera::sendKeyInput(const opera::msg::KeyInput& key_input)
{
    BROWSER_LOG(
        INFO,
        "event_type: %s, virtual_key: %s, key_modifiers: %s, key_value: %s",
        opera::msg::KeyInput::ToString(key_input.event_type).c_str(),
        opera::msg::KeyInput::ToString(key_input.virtual_key).c_str(),
        opera::msg::KeyInput::ToString(key_input.key_modifiers).c_str(),
        key_input.key_value.c_str());

    m_opera->post(opera::msg::KeyInput(key_input.event_type, key_input.virtual_key));
}

void HisenseOpera::moveInHistory( const opera::Handle& window, const int direction, unsigned int offset )
{
    BROWSER_LOG( INFO, "window : %s, direction : %d, offset : %d", window.str().c_str(), direction, offset );
    opera::msg::MoveInHistory::Result result =
        m_opera->post(opera::msg::MoveInHistory(window,
            (direction == 0) ? opera::msg::MoveInHistory::HISTORY_BACK : opera::msg::MoveInHistory::HISTORY_FORWARD, offset)).get();
    if ( result.error != opera::msg::MoveInHistory::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to set history window : %s, error : %s", window.str().c_str(), result.errorString().c_str() );
}

void HisenseOpera::commitIME( const opera::Handle& window, const std::string& commit_text, bool submit )
{
    BROWSER_LOG( INFO, "Handle : %s, commit_text : %s, submit : %d", window.str().c_str(), commit_text.c_str(), submit );
    opera::msg::CommitIME::Result result = m_opera->post( opera::msg::CommitIME( window, commit_text, true, submit ) ).get();
    if ( result.error != opera::msg::CommitIME::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to commit IME window : %s, error : %s", window.str().c_str(), result.errorString().c_str() );
}

void HisenseOpera::cancelIME( const opera::Handle& window )
{
    BROWSER_LOG( INFO, "Handle : %s", window.str().c_str() );
    opera::msg::CancelIME::Result result = m_opera->post( opera::msg::CancelIME( window ) ).get();
    if ( result.error != opera::msg::CancelIME::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to cancel IME window : %s, error : %s", window.str().c_str(), result.errorString().c_str() );
}

void HisenseOpera::clearCookies()
{
    BROWSER_LOG( INFO, "" );
    m_opera->post( opera::msg::ClearCookies() );
}

void HisenseOpera::setBounds( const opera::Handle& id, int x, int y, int width, int height )
{
    BROWSER_LOG( INFO, "Handle : %s", id.str().c_str() );
    opera::msg::SetBounds::Result result = m_opera->post( opera::msg::SetBounds( id, opera::Rect( x, y, width, height ) ) ).get();
    if ( result.error != opera::msg::SetBounds::NO_ERR ) {
        BROWSER_LOG( ERROR, "Failed to set bounds window or view : %s, error : %s", id.str().c_str(), result.errorString().c_str() );
    }
}

void HisenseOpera::setStackAbove( const opera::Handle& id, const opera::Handle& reference_id )
{
    BROWSER_LOG( INFO, "Handle : %s, reference_id : %s", id.str().c_str(), reference_id.str().c_str() );
    opera::msg::StackAbove::Result result = m_opera->post( opera::msg::StackAbove( id, reference_id ) ).get();
    if ( result.error != opera::msg::StackAbove::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to set Stack above error : %s", result.errorString().c_str() );
}

void HisenseOpera::setStackBloew( const opera::Handle& id, const opera::Handle& reference_id )
{
    BROWSER_LOG( INFO, "Handle : %s, reference_id : %s", id.str().c_str(), reference_id.str().c_str() );
    opera::msg::StackBelow::Result result = m_opera->post( opera::msg::StackBelow( id, reference_id ) ).get();
    if ( result.error != opera::msg::StackBelow::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to set Stack below error : %s", result.errorString().c_str() );
}

void HisenseOpera::setStackAtTop( const opera::Handle& id )
{
    BROWSER_LOG( INFO, "Handle : %s", id.str().c_str() );
    opera::msg::StackAtTop::Result result = m_opera->post( opera::msg::StackAtTop( id ) ).get();
    if ( result.error != opera::msg::StackAtTop::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to set stack at top error : %s", result.errorString().c_str() );
}

void HisenseOpera::setStackAtBottom( const opera::Handle& id )
{
    BROWSER_LOG( INFO, "Handle : %s", id.str().c_str() );
    opera::msg::StackAtBottom::Result result = m_opera->post( opera::msg::StackAtBottom( id ) ).get();
    if ( result.error != opera::msg::StackAtBottom::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to set stack at bottom error : %s", result.errorString().c_str() );
}

int HisenseOpera::getCurrentHistoryIndex( const opera::Handle& id )
{
    BROWSER_LOG( INFO, "Handle : %s", id.str().c_str() );
    opera::msg::GetCurrentHistoryIndex::Result result = m_opera->post(opera::msg::GetCurrentHistoryIndex( id ) ).get();
    if ( result.error != opera::msg::GetCurrentHistoryIndex::NO_ERR ) {
        BROWSER_LOG( ERROR, "Failed to get history index window : %s, error : %s", id.str().c_str(), result.errorString().c_str() );
        return 0;
    } else {
        return result.index;
    }
}

int HisenseOpera::getHistoryCount( const opera::Handle& id )
{
    BROWSER_LOG( INFO, "Handle : %s", id.str().c_str() );
    opera::msg::GetHistoryCount::Result result = m_opera->post(opera::msg::GetHistoryCount( id ) ).get();
    if ( result.error != opera::msg::GetHistoryCount::NO_ERR ) {
        BROWSER_LOG( ERROR, "Failed to get history count window : %s, error : %s", id.str().c_str(), result.errorString().c_str() );
        return 0;
    } else {
        return result.count;
    }
}


void HisenseOpera::showCursor( const opera::Handle& id )
{
    BROWSER_LOG( INFO, "Handle : %s", id.str().c_str() );
    opera::msg::ShowCursor::Result result = m_opera->post(opera::msg::ShowCursor( id )).get();
    if ( result.error != opera::msg::ShowCursor::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to show cursor error : %s", result.errorString().c_str() );
}

void HisenseOpera::hideCursor( const opera::Handle& id )
{
    BROWSER_LOG( INFO, "Handle : %s", id.str().c_str() );
    opera::msg::HideCursor::Result result = m_opera->post(opera::msg::HideCursor( id )).get();
    if ( result.error != opera::msg::HideCursor::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to hide cursor error : %s", result.errorString().c_str() );
}

void HisenseOpera::sendMouseInput( int x, int y,
                                        const opera::msg::MouseInput::Type& event_type,
                                        const opera::msg::MouseInput::Button& button,
                                        const opera::msg::MouseInput::Modifier& modifiers )
{
    BROWSER_LOG( DETAIL, "x : %d, y : %d, event_type : %d, button : %d, modifiers : %d", x , y, event_type, button, modifiers );
    m_opera->post(opera::msg::MouseInput(x, y, event_type, button, modifiers));
}

void HisenseOpera::sendKeyInput( const opera::msg::KeyInput::Type& event_type,
                                     const opera::msg::KeyInput::Key& virtual_key,
                                     const opera::msg::KeyInput::Modifier& key_modifiers,
                                     bool key_repeat, const std::string& key_value,
                                     const opera::Handle& forced_target)
{
    BROWSER_LOG( DETAIL, "event_type : %d, virtual_key : %d, modifiers : %d, key_repeat : %d, key_value : %s, forced_target : %s",
                 event_type, virtual_key, key_modifiers, key_repeat, key_value.c_str(), forced_target.str().c_str() );
    m_opera->post(opera::msg::KeyInput( event_type, virtual_key, key_modifiers, key_repeat, key_value, forced_target ));
}

//void HisenseOpera::setHTMLViewOpacity( const opera::Handle& view, float opacity )
//{
//    BROWSER_LOG( INFO, "Handle : %s, opacity : %f", view.str().c_str(), opacity  );
//    opera::msg::SetHTMLViewOpacity::Result result = m_opera->post( opera::msg::SetHTMLViewOpacity( view, opacity ) ).get();
//    if ( result.error != opera::msg::SetHTMLViewOpacity::NO_ERR )
//        BROWSER_LOG( ERROR, "Failed to set HTMLView opacity view : %s, error : %s", view.str().c_str(), result.errorString().c_str() );
//}

void HisenseOpera::setTimeShiftMode( const opera::Handle& window, bool time_shift_mode )
{
    BROWSER_LOG( INFO, "Handle : %s, time_shift_mode : %d", window.str().c_str(), time_shift_mode  );
    opera::msg::SetTimeShiftMode::Result result = m_opera->post( opera::msg::SetTimeShiftMode( window, time_shift_mode ) ).get();
    if ( result.error != opera::msg::SetTimeShiftMode::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to set time shift mode window : %s, error : %s", window.str().c_str(), result.errorString().c_str() );
}

void HisenseOpera::reload( const opera::Handle& window )
{
    BROWSER_LOG( INFO, "Handle : %s", window.str().c_str() );
    opera::msg::Reload::Result result = m_opera->post( opera::msg::Reload( window ) ).get();
    if ( result.error != opera::msg::Reload::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to reload window : %s, error : %s", window.str().c_str(), result.errorString().c_str() );
}

void HisenseOpera::exitFullscreenMode( const opera::Handle& window )
{
    BROWSER_LOG( INFO, "Handle : %s", window.str().c_str() );
    opera::msg::ExitFullscreenMode::Result result = m_opera->post(
        opera::msg::ExitFullscreenMode( window ) ).get();
    if ( result.error != opera::msg::ExitFullscreenMode::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to exit full screen mode window : %s, error : %s",
                     window.str().c_str(), result.errorString().c_str() );
}

void HisenseOpera::sendPlatformEvent( const opera::Handle& window, const std::string& message )
{
    BLOGV("Handle : %s,%s", window.str().c_str(), message.c_str() );
    //BROWSER_LOG( DETAIL, "text : %s", message.c_str() );
    opera::msg::SendPlatformEvent::Result result = m_opera->post( opera::msg::SendPlatformEvent( window, message ) ).get();
    if ( result.error != opera::msg::SendPlatformEvent::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to set platform message window : %s, error : %s",
                     window.str().c_str(), result.errorString().c_str() );
}

void HisenseOpera::setInputMethod( const opera::Handle& window, const opera::msg::SetInputMethod::InputMethod& input_method )
{
    BROWSER_LOG( INFO, "Handle : %s, input_method : %d", window.str().c_str(), (int)input_method );
    opera::msg::SetInputMethod::Result result = m_opera->post( opera::msg::SetInputMethod( window, input_method ) ).get();
    if ( result.error != opera::msg::SetInputMethod::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to set input method window : %s, error : %s",
                     window.str().c_str(), result.errorString().c_str() );
}


void HisenseOpera::parseAIT(const opera::Handle& window,
                            unsigned short original_network_id,
                            unsigned short transport_stream_id,
                            unsigned short service_id,
                            const std::string& ccid,
                            const std::vector<uint8_t> & data)
{
    BROWSER_LOG( INFO, "Handle : %s", window.str().c_str() );
    opera::msg::ParseAIT::Result result =  m_opera->post(
        opera::msg::ParseAIT(window, original_network_id, transport_stream_id, service_id, ccid, data)).get();
    if ( result.error != opera::msg::ParseAIT::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to parse AIT, error : %s", result.errorString().c_str() );
}

void HisenseOpera::invalidateAIT( const opera::Handle& window )
{
    BROWSER_LOG( INFO, "Handle : %s", window.str().c_str() );
    opera::msg::InvalidateAIT::Result result =  m_opera->post( opera::msg::InvalidateAIT( window ) ).get();
    if ( result.error != opera::msg::InvalidateAIT::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to invalidate AIT, error : %s", result.errorString().c_str() );
}

void HisenseOpera::injectRemoteXmlAIT( const opera::Handle& window,
                                           const std::string& xml_ait_url,
                                           const std::string& ccid )
{
    BROWSER_LOG( INFO, "Handle : %s, xml_ait_url : %s, ccid : %s", window.str().c_str(), xml_ait_url.c_str(), ccid.c_str() );
    opera::msg::InjectRemoteXmlAIT::Result result =  m_opera->post(
        opera::msg::InjectRemoteXmlAIT( window, xml_ait_url, ccid ) ).get();
    if ( result.error != opera::msg::InjectRemoteXmlAIT::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to inject remote xml ait, error : %s", result.errorString().c_str() );
}


void HisenseOpera::setBroadbandStatus( const opera::Handle& window, const opera::msg::SetBroadbandStatus::Status& status )
{
    BROWSER_LOG( INFO, "Handle : %s, status : %d", window.str().c_str(), (int)status );
    opera::msg::SetBroadbandStatus::Result result =  m_opera->post(
        opera::msg::SetBroadbandStatus( window, status ) ).get();
    if ( result.error != opera::msg::SetBroadbandStatus::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to set Broadband status, error : %s", result.errorString().c_str() );
}

void HisenseOpera::loadBroadcastRelatedApplication( const opera::Handle& window, unsigned short app_id, unsigned int org_id )
{
    BROWSER_LOG( INFO, "Handle : %s, app_id : %d, org_id : %d", window.str().c_str(), (int)app_id, (int)org_id );
    opera::msg::LoadBroadcastRelatedApplication::Result result =  m_opera->post(
        opera::msg::LoadBroadcastRelatedApplication( window, app_id, org_id ) ).get();
    if ( result.error != opera::msg::LoadBroadcastRelatedApplication::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to load broadcast related application, error : %s", result.errorString().c_str() );
}

void HisenseOpera::loadBroadcastIndependentApplication( const opera::Handle& window, const std::string& app_url )
{
    BROWSER_LOG( INFO, "Handle : %s, app_url : %s", window.str().c_str(), app_url.c_str() );
    opera::msg::LoadBroadcastIndependentApplication::Result result =  m_opera->post(
        opera::msg::LoadBroadcastIndependentApplication( window, app_url ) ).get();
    if ( result.error != opera::msg::LoadBroadcastIndependentApplication::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to load broadcast independent application, error : %s", result.errorString().c_str() );
}

void HisenseOpera::raiseDSMCCStreamEvent( const opera::Handle& window,
                                                 unsigned int listener_id,
                                                 unsigned int video_id,
                                                 const std::string& event_name,
                                                 const std::string& event_data,
                                                 const std::string& event_text,
                                                 const std::string& event_status )
{
    BROWSER_LOG( INFO, "Handle : %s, listener_id : %d, video_id : %d, event_name : %s, event_data : %s, event_data : %s, event_status : %s",
                window.str().c_str(), listener_id, video_id, event_name.c_str(), event_data.c_str(), event_data.c_str(), event_status.c_str() );
    opera::msg::RaiseDSMCCStreamEvent::Result result =  m_opera->post(
        opera::msg::RaiseDSMCCStreamEvent( window, listener_id, video_id, event_name,
                                           event_data,event_text, event_status.c_str() ) ).get();
    if ( result.error != opera::msg::RaiseDSMCCStreamEvent::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to raise DSMCC stream event, error : %s", result.errorString().c_str() );
}


void HisenseOpera::terminateApplication( const opera::Handle& window, bool launch_ait_autostart )
{
    BROWSER_LOG( INFO, "Handle : %s", window.str().c_str() );
    opera::msg::TerminateApplication::Result result =  m_opera->post(
        opera::msg::TerminateApplication( window, launch_ait_autostart ) ).get();
    if ( result.error != opera::msg::TerminateApplication::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to terminate application, error : %s", result.errorString().c_str() );
}

void HisenseOpera::activeOIPFApplication( const opera::Handle& window, unsigned int application_id, bool gain_focus )
{
    BROWSER_LOG( INFO, "Handle : %s, application_id : %d, gain_focus : %d", window.str().c_str(), application_id, gain_focus );
    opera::msg::ActivateOIPFApplication::Result result = m_opera->post(
        opera::msg::ActivateOIPFApplication( window, application_id, gain_focus ) ).get();
    if ( result.error != opera::msg::ActivateOIPFApplication::NO_ERR )
        BROWSER_LOG( ERROR, "Failed to active OIPF application, error : %s", result.errorString().c_str() );
}

void HisenseOpera::clearApplicationCache()
{
    BROWSER_LOG( INFO, "" );
    m_opera->post( opera::msg::ClearApplicationCache() );
}

void HisenseOpera::clearDiskCache()
{
    BROWSER_LOG( INFO, "" );
    m_opera->post( opera::msg::ClearDiskCache() );
}

void HisenseOpera::clearLocalStorage()
{
    BROWSER_LOG( INFO, "" );
    m_opera->post( opera::msg::ClearLocalStorage() );
}

int HisenseOpera::getMemoryInformation()
{
    BROWSER_LOG( INFO, "" );
    opera::msg::GetBrowserMemoryInformation::Result result = m_opera->post(
        opera::msg::GetBrowserMemoryInformation() ).get();
    return result.allocated;
}

int HisenseOpera::getWindowMemoryInformation( const opera::Handle& window )
{
    BROWSER_LOG( INFO, "Handle : %s", window.str().c_str() );
    opera::msg::GetWindowMemoryInformation::Result result = m_opera->post(
        opera::msg::GetWindowMemoryInformation() ).get();
    if ( result.error != opera::msg::GetWindowMemoryInformation::NO_ERR ) {
        BROWSER_LOG( ERROR, "Failed to get window memory information, error : %s", result.errorString().c_str() );
        return -1;
    } else {
        return result.allocated;
    }
}



void HisenseOpera::pause() {
    if ( m_operaController ) {
        m_operaController->pause();
        // Remove all event listener

        // TODO(sunkai5): hbbtv may be messed up
      //  RemoveAllEventHandler();
    }
}
void HisenseOpera::removeAllEventListeners()
{
   m_operaController->RemoveAllEventHandler();
}
void HisenseOpera::resume() {
    if ( m_operaController )
        m_operaController->resume();
}

opera::components::OperaController* HisenseOpera::getOperaController() {
    return m_operaController;
}

opera::Opera& HisenseOpera::getOpera() {
    return *m_opera;
}

std::string HisenseOpera::getStatusPageUrl() {
    return m_operaStatusPage;
}

std::string HisenseOpera::getFoxxumUrl()
{
    return m_foxxum_url;
}
std::string HisenseOpera::getNetranageUrl()
{
    return m_netrange_url;
}

std::string HisenseOpera::getErrorPage()
{
    return m_error_page;
}

void HisenseOpera::browserMemoryEvent( opera::evt::BrowserMemoryEvent event )
{
    switch( event.state )
    {
        case opera::evt::BrowserMemoryEvent::OK:
            BROWSER_LOG( ALWAYS, "Enough Browser Memory!!!" );
            break;
        case opera::evt::BrowserMemoryEvent::LOW:
            BROWSER_LOG( ERROR, "Low Browser Memory!!!" );
            break;
        case opera::evt::BrowserMemoryEvent::OOM:
            BROWSER_LOG( ERROR, "Browser Memory OOM!!!" );
            break;
        default:
            BROWSER_LOG( ERROR, "Other!!!" );
    }
}

void HisenseOpera::onStopPlatformMessage( opera::evt::PlatformMessage event )
{

    Json::Reader reader;
    Json::Value  value;
    bool parse_result = reader.parse(event.message, value);
    int control = 0;
    if(parse_result)
    {
        control = value.get("BrowserControl", 0).asInt();
    }
    if (control == StopCurrentMode) {
        browser().getTaskManager().postTask( hisense::AsyncBrowserTask::create(&browser(),"pause"));
    }
}

void HisenseOpera::ScriptRequestCloseWindow(opera::evt::ScriptRequestCloseWindow event)
{

    BLOGI("Got ScriptRequestCloseWindow window_id %s opened_by_dom %d url %s",
        event.id.str().c_str(), event.opened_by_dom, event.url.c_str());

    //FIXME: What is this for?
    if(event.url.find("closeWindow=1")!=std::string::npos)
    {
        BLOGE("Enter the Opera store app window!\n");
    }
    else
    {
        browser().getTaskManager().postTask( hisense::AsyncBrowserTask::create(&browser(),"pause"));
    }
}

void HisenseOpera::disableScreenReaderHighlight(const opera::Handle& window)
{
    setPref(window, "chromevox_enabled.override", "true");
    setPref(window, "accessibility_cursor_highlight_enabled", "false");
    setPref(window, "accessibility_caret_highlight_enabled", "false");
    setPref(window, "accessibility_focus_highlight_enabled", "false");
    setPref(window, "chromevox_enabled.override", "false");
}

