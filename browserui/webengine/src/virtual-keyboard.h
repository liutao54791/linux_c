#ifndef BROWSER_VIRTUAL_KEYBOARD_H
#define BROWSER_VIRTUAL_KEYBOARD_H

#include <opera/Opera.h>
#include <opera/events/IMECancelled.h>
#include <opera/events/IMEStart.h>
#include <opera/events/PlatformMessage.h>
#include <opera/messages/KeyInput.h>

#include "browser_type.h"
#include "browser-opera.h"


#include "browser_component.h"

//bool EnterKeyProc(const opera::msg::KeyInput& key);
//void MouseClickProc(const opera::msg::MouseInput& mouse_input);

class VirtualKeyboard : public BrowserComponent
{
public:
    // static VirtualKeyboard *m_instance;
    VirtualKeyboard(BrowserImpl* browserImpl);
    ~VirtualKeyboard();
    bool sendKeyInput(const opera::msg::KeyInput& key);
    bool EnterKeyProc(const opera::msg::KeyInput& key);
    void MouseClickProc(const opera::msg::MouseInput& mouse_input);
    BrowserStatus start();
    BrowserStatus stop();

    std::string getUrl()
    {
        return m_startUrl;
    }
    bool isShow();
    void leave();
    void enter(opera::Handle window);
    void setTargetWindow(opera::Handle window);
    void setTargetWindows(opera::Handle window1, opera::Handle window2);

    //    opera::Handle getWindow(){
    //        return m_keyboardWindow;
    //    }

private:
    //    typedef enum
    //    {
    //        NO_IME,
    //        IME_READY,
    //        VKB_RUNNING
    //    } Status;

    void OnIMEStart(opera::evt::IMEStart ime_msg);
    void OnIMECancelled(opera::evt::IMECancelled ime_msg);
    void OnPlatformMessage(opera::evt::PlatformMessage platform_msg);
    void CreateUI();
    void Show();
    void CommitEdit(std::vector<std::string> &arguments);
    void CancleEdit();

    HisenseOpera &m_opera;
    opera::Handle m_keyboardWindow;
    //  opera::Handle m_window;
    std::vector<opera::Handle> m_targetWindows;
    opera::Handle m_currentTarget;
    std::string m_text;
    std::string m_type;
    //  Status m_status;
    bool m_enable_vkb;
    bool m_page_at_input_mode;
    bool m_isShown;
    //  bool m_show;
    bool m_last_input_is_enter_or_click;
    std::string m_startUrl;
};

#endif
