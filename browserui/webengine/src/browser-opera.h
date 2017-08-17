#ifndef HISENSE_OPERA_H
#define HISENSE_OPERA_H

#include <memory>

#include <opera/components/opera_controller.h>

#include <opera/messages/CreateHbbTVWindow.h>
#include <opera/messages/SetInputMethod.h>
#include <opera/messages/SetBroadbandStatus.h>
#include <opera/messages/KeyInput.h>
#include <opera/messages/MouseInput.h>

#include <opera/events/BrowserMemoryEvent.h>
#include <opera/events/PlatformMessage.h>
#include <opera/events/ScriptRequestCloseWindow.h>

#include "browser_type.h"
#include "browser_component.h"
#undef WINDOW_TRANS

namespace opera {
class Opera;

namespace msg {
class KeyInput;
}
}

typedef struct
{
    int x;
    int y;
    int width;
    int height;
} BrowserRect_t;

class HisenseOpera : public BrowserComponent
{
public:
    HisenseOpera(BrowserImpl* browserImpl);
    ~HisenseOpera();

    BrowserStatus start();

    BrowserStatus initialize();

    // the event handler added by this funciton can be removed by RemoveAllEventHandler()
    template <typename TEvent, typename TReceiver>
    void AddEventHandler(void (TReceiver::*handler)(TEvent), TReceiver* receiver);

    // the event handler added by this funciton can't be removed by RemoveAllEventHandler()
    template <typename TEvent, typename TReceiver>
    void AddStationaryEventHandler(void (TReceiver::*handler)(TEvent), TReceiver* receiver);

    void stop();

    opera::Handle createStoreWindow(const BrowserStoreWindowType& type, const std::string& url);
    opera::Handle createHBBTVWindow(const std::string ua_extra_capabilities,
                                    const std::string ua_vendor_name,
                                    const std::string ua_model_name,
                                    const std::string ua_software_version,
                                    const std::string ua_hardware_version,
                                    const std::string ua_family_name,
                                    unsigned short application_profile,
                                    std::vector<opera::msg::CreateHbbTVWindow::PlatformProfile> platform_profiles,
                                    int key_set_mask);
    opera::Handle createStatusWindow(const opera::Handle& group_with = opera::Handle(-1));
    //   opera::Handle createStatusHTMLView( const opera::Handle window );
    opera::Handle createBrowserUIWindow(const std::string& startUrl);
    //    opera::Handle createHTMLView(const opera::Handle& window,
    //                                     const std::string& url,
    //                                     int x = 0, int y = 0, int width = 0, int height = 0,
    //                                     bool visible = true);

    opera::Handle createWindow(
#if 0 //def WINDOW_TRANS
            bool transparent = true,
#endif
            bool visible = true, bool focused = true,
                               const BrowserRect_t& rect = {0, 0, -1, -1}, const opera::Handle& group_with = opera::Handle(-1),
                               int priority = 0, bool private_browsing = false, const std::string& browser_context = "");
    void createOIPFApplication(opera::Handle& window, std::string app_url);

    void show(const opera::Handle& window);
    void hide(const opera::Handle& window);
    void focus(const opera::Handle& window);
    void blur();
    // factor form 0.25->5.0
    BrowserStatus setPageZoom(const opera::Handle& window, double factor);
    void getCurrentScroll(const opera::Handle& window, int& x, int& y);
    void getMaxScroll(const opera::Handle& window, int& x, int& y);
    void setScroll(const opera::Handle& window, int x, int y);
    void stopLoading(const opera::Handle& window);
    void destroyWindow(const opera::Handle& window);
    //    void destroyHTMLView( const opera::Handle& view );
    void loadURL(const opera::Handle& window, const std::string& url);
    // 0 for back, 1 for forward
    void moveInHistory(const opera::Handle& window, const int direction, unsigned int offset = 1);
    void commitIME(const opera::Handle& window, const std::string& commit_text, bool submit = false);
    void cancelIME(const opera::Handle& window);
    void reload(const opera::Handle& window);
    void exitFullscreenMode(const opera::Handle& window);
    void sendPlatformEvent(const opera::Handle& window, const std::string& message);
    void setInputMethod(const opera::Handle& window, const opera::msg::SetInputMethod::InputMethod& input_method);
    void setBounds(const opera::Handle& window, int x = 0, int y = 0, int width = 0, int height = 0);
    void setStackAbove(const opera::Handle& id, const opera::Handle& reference_id);
    void setStackBloew(const opera::Handle& id, const opera::Handle& reference_id);
    void setStackAtTop(const opera::Handle& id);
    void setStackAtBottom(const opera::Handle& id);
    int getCurrentHistoryIndex(const opera::Handle& id);
    int getHistoryCount(const opera::Handle& id);

    void showCursor(const opera::Handle& id);
    void hideCursor(const opera::Handle& id);
    void sendMouseInput(int x, int y,
                        const opera::msg::MouseInput::Type& event_type,
                        const opera::msg::MouseInput::Button& button = opera::msg::MouseInput::NONE,
                        const opera::msg::MouseInput::Modifier& modifiers = opera::msg::MouseInput::MODIFIER_NONE);
    void sendKeyInput(const opera::msg::KeyInput::Type& event_type,
                      const opera::msg::KeyInput::Key& virtual_key,
                      const opera::msg::KeyInput::Modifier& key_modifiers = opera::msg::KeyInput::MODIFIER_NONE,
                      bool key_repeat = false, const std::string& key_value = "",
                      const opera::Handle& forced_target = opera::Handle(-1));

    void setPref(const opera::Handle& window, const std::string& key, const std::string& value);
    // return "ERROR" string when error
    std::string getPref(const opera::Handle& window, const std::string& key);

    void sendKeyInput(const opera::msg::KeyInput& key_input);
    void clearCookies();
    //    void setHTMLViewOpacity( const opera::Handle& view, float opacity );
    void setTimeShiftMode(const opera::Handle& window, bool time_shift_mode);

    void parseAIT(const opera::Handle& window_,
                  unsigned short original_network_id,
                  unsigned short transport_stream_id,
                  unsigned short service_id,
                  const std::string& ccid,
                  const std::vector<uint8_t> & data);
    void invalidateAIT(const opera::Handle& window);
    void injectRemoteXmlAIT(const opera::Handle& window,
                            const std::string& xml_ait_url,
                            const std::string& ccid = "");
    void setBroadbandStatus(const opera::Handle& window,
                            const opera::msg::SetBroadbandStatus::Status& status);
    void loadBroadcastIndependentApplication(const opera::Handle& window,
                                             const std::string& app_url);
    void loadBroadcastRelatedApplication(const opera::Handle& window,
                                         unsigned short app_id,
                                         unsigned int org_id);
    void raiseDSMCCStreamEvent(const opera::Handle& window,
                               unsigned int listener_id,
                               unsigned int video_id,
                               const std::string& event_name,
                               const std::string& event_data,
                               const std::string& event_text,
                               const std::string& event_status);

    void terminateApplication(const opera::Handle& window, bool launch_ait_autostart = false);
    void activeOIPFApplication(const opera::Handle& window, unsigned int application_id, bool gain_focus = true);

    void clearApplicationCache();
    void clearDiskCache();
    void clearLocalStorage();
    //in bytes
    int getMemoryInformation();
    //in bytes, -1 for error
    int getWindowMemoryInformation(const opera::Handle& window);

    opera::components::OperaController* getOperaController();
    opera::Opera& getOpera();

    void pause();
    void resume();
    void removeAllEventListeners();
    std::string getStatusPageUrl();
    std::string getFoxxumUrl();
    std::string getNetranageUrl();
    std::string getErrorPage();
    void disableScreenReaderHighlight(const opera::Handle& window);

private:
    void browserMemoryEvent(opera::evt::BrowserMemoryEvent event);
    void onStopPlatformMessage(opera::evt::PlatformMessage message);
    void ScriptRequestCloseWindow(opera::evt::ScriptRequestCloseWindow event);

private:


    std::shared_ptr<opera::Opera> m_opera;
    opera::components::OperaController* m_operaController;

    std::string m_operaStoreUA;
    std::string m_operaStatusPage;
    std::string m_operaStoreDefaultUrl;
    std::string m_browerUIUserAgent;
    std::string m_netrange_url;
    std::string m_foxxum_url;
    std::string m_error_page;

    void GenerateDeviceID();
    void SetStoragePassword();
    void SetTVStoreDeviceModelName();
};

template <typename TEvent, typename TReceiver>
void HisenseOpera::AddEventHandler(void (TReceiver::*handler)(TEvent), TReceiver* receiver)
{
    m_operaController->AddEventHandler(handler, receiver);
}

template <typename TEvent, typename TReceiver>
void HisenseOpera::AddStationaryEventHandler(void (TReceiver::*handler)(TEvent), TReceiver* receiver)
{
    m_operaController->AddStationaryEventHandler(handler, receiver);
}

#endif
