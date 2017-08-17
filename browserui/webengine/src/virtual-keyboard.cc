#include <vector>
#include <sstream>
#include <iostream>

#include "language.h"
#include "sysinfo.h"
#include "browser_debug.h"
#include "virtual-keyboard.h"
#include "browser-impl.h"
#include "graphics.h"
#include "browser_mouse.h"
#include <json/json.h>
#include "configuration.h"

extern bool keyboard_search_flag;
extern bool isSpecialURL;
namespace {

std::string OperaDirFileURL(const std::string& file)
{
    std::string opera_dir_path = getenv("OPERA_DIR");
    return "file://" + opera_dir_path + '/' + file;
}

void SplitString(const std::string& s, char delim,
        std::vector<std::string>& elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
        elems.push_back(item);
}

void ParseCommand(const std::string commandString,
        std::string& command,
        std::vector<std::string>& elems)
{
    SplitString(commandString, ';', elems);
    if (elems.size() > 0) {
        command = elems[0];
        elems.erase(elems.begin());
    }
}
}

VirtualKeyboard::VirtualKeyboard(BrowserImpl* browserImpl)
: BrowserComponent(browserImpl)
, m_opera(browser().getHisenseOpera())
, m_text("")
, m_type("")
, m_enable_vkb(true)
, m_page_at_input_mode(false)
, m_isShown(false)
, m_startUrl("")
, m_last_input_is_enter_or_click(false)
{

    TRACE_HERE(0);
}

VirtualKeyboard::~VirtualKeyboard()
{
    TRACE_HERE(0);
}

BrowserStatus VirtualKeyboard::start()
{
    TRACE_HERE(0);
    return BrowserOK;
}

BrowserStatus VirtualKeyboard::stop()
{
    TRACE_HERE(0);
    if (m_keyboardWindow.IsValid()) {
        m_opera.destroyWindow(m_keyboardWindow);
    }

    return BrowserOK;
}

void VirtualKeyboard::leave()
{
    TRACE_HERE(0);
    CancleEdit();
    // m_instance = NULL;

    if (m_keyboardWindow.IsValid()) {

        m_opera.destroyWindow(m_keyboardWindow);

    }
}

void VirtualKeyboard::enter(opera::Handle window)
{
    BROWSER_LOG(INFO, "");
    m_enable_vkb = true;
    m_text = "";
    m_type = "0";
    setTargetWindow(window);
    //   m_status = NO_IME;
    //  m_window = window;
    m_opera.AddEventHandler(&VirtualKeyboard::OnIMEStart, this);
    m_opera.AddEventHandler(&VirtualKeyboard::OnIMECancelled, this);
    m_opera.AddEventHandler(&VirtualKeyboard::OnPlatformMessage, this);
    // m_instance = this;

    CreateUI();
}

void VirtualKeyboard::setTargetWindow(opera::Handle window)
{
    m_currentTarget = opera::Handle(-1);
    m_targetWindows.clear();
    m_targetWindows.push_back(window);
}

void VirtualKeyboard::setTargetWindows(opera::Handle window1, opera::Handle window2)
{
    m_currentTarget = opera::Handle(-1);
    m_targetWindows.clear();
    m_targetWindows.push_back(window1);
    m_targetWindows.push_back(window2);
}

bool VirtualKeyboard::isShow()
{
    //return (m_status == VKB_RUNNING);
    return m_isShown;
}

bool VirtualKeyboard::EnterKeyProc(const opera::msg::KeyInput& key)
{

    if (key.virtual_key == opera::msg::KeyInput::OMI_KEY_ENTER) {
        m_last_input_is_enter_or_click = true;
        BLOGI("Get Enter Key Input....  m_last_input_is_enter_or_click: %d", m_last_input_is_enter_or_click);
        //return true;
    } else {
        m_last_input_is_enter_or_click = false;
        BLOGI("Get Other Key Input....  keym_last_input_is_enter_or_clickfalg: %d", m_last_input_is_enter_or_click);
        //return false;
    }
    TRACE_HERE(0);
    return m_last_input_is_enter_or_click;
}

void VirtualKeyboard::MouseClickProc(const opera::msg::MouseInput& mouse_input)
{
    // TRACE_HERE(0);
    if (mouse_input.event_type == opera::msg::MouseInput::RELEASE) {
        return;
    }
    if (mouse_input.event_type == opera::msg::MouseInput::PRESS) {
        if ((mouse_input.button == opera::msg::MouseInput::LEFT)
                && (mouse_input.modifiers == opera::msg::MouseInput::MODIFIER_NONE)) {
            m_last_input_is_enter_or_click = true;
        } else {
            m_last_input_is_enter_or_click = false;
        }
        BLOGI("left m_last_input_is_enter_or_click: %d", m_last_input_is_enter_or_click);
        return;
    } else {
        m_last_input_is_enter_or_click = false;
        return;
    }
}

void VirtualKeyboard::OnIMEStart(opera::evt::IMEStart ime_msg)
{
    BLOGI("IMEStart:---  id=%s,pos:(%d,%d),type:%s,trigger:%s,format:%s,text=%s,",
          ime_msg.id.str().c_str(),
          ime_msg.caret_position.x,
          ime_msg.caret_position.y,
          opera::evt::IMEStart::ToString(ime_msg.input_type).c_str(),
          opera::evt::IMEStart::ToString(ime_msg.trigger_type).c_str(),
          opera::evt::IMEStart::ToString(ime_msg.input_format).c_str(),
          ime_msg.original_text.c_str()
          );
    BLOGI("m_last_input_is_enter_or_click: %d", m_last_input_is_enter_or_click);
    //No IME supported:
    if (!m_enable_vkb) {
        BLOGI("VKB is disabled");
        return;
    }

    opera::Handle target = ime_msg.id;
    //TODO: check for the ignored window:
    //Ignore IME event from hibrowserUI,IMEItself,and some other windows.
    if (m_keyboardWindow == target) {
        BLOGI("Skip IME triggered by virtual keyboard pages");
        return;
    }
    //FIXME:We should disable changing of targets when deal with IME start event:
    bool target_accepted = false;
    for (int i = 0; i < m_targetWindows.size(); i++) {
        if (m_targetWindows[i] == target) {
            target_accepted = true;
            m_currentTarget = target;
            break;
        }
    }


    if (!target_accepted) {
        BLOGW("Ignore IME started from un-targeted windows:target: %s vs %s",
              m_currentTarget.str().c_str(), target.str().c_str());
        return;
    }

    //We keep track the foucus status:
    //The focus is in input box:
    m_page_at_input_mode = true;


    //Check for exceptins:
    if (!m_last_input_is_enter_or_click)
        //(ime_msg.trigger_type != opera::evt::IMEStart::TRIGGER_ENTER
        // &&ime_msg.trigger_type != opera::evt::IMEStart::TRIGGER_CURSOR_MOVE
        //)
    {
        //BROWSER_LOG(ERROR,"We Only Accept IME triggered by Enter or Mouse");
        BLOGI("m_last_input_is_enter_or_click is false. Do not start IME");
        return;
    }
    BLOGI("Start IME for %s", target.str().c_str());

    if (isShow()) {
        BLOGI("IME Already Started.");
        return;
    }
    //Now Start IME window:

    if (!m_keyboardWindow.IsValid())
        CreateUI();

    m_text = ime_msg.original_text;
    //TODO: support other types:
    m_type = (ime_msg.input_type != opera::evt::IMEStart::INPUT_PASSWORD) ? "0" : "1";

    ime_msg.Accept(ime_msg.id);
    Show();

}

void VirtualKeyboard::OnIMECancelled(opera::evt::IMECancelled ime_msg)
{
    BLOGI("IME Cancel:-- ");
    m_page_at_input_mode = false;

}

void VirtualKeyboard::Show()
{
    BLOGI("Show the VKB!\n");

    m_opera.show(m_keyboardWindow);
    m_opera.focus(m_keyboardWindow);

    BROWSER_LANG_ID lang_id = browser().getConfiguration().getTVLanguageID();

    BLOGI("LANGID:%d", lang_id);
    std::string virtualKeyboardLang = browser().getLanguage().getNameForId(lang_id);

    // "vkbInfo;type;xPos;yPos;lang;msg"
    std::string ime_info = "";
    ime_info = ime_info + "{"
            + "\"cmd\":\"vkbShow\","
            + "\"type\":\"" + m_type + "\","
            + "\"x\":" + "0" + ","
            + "\"y\":" + "0" + ","
            + "\"lang\":\"" + virtualKeyboardLang + "\","
            + "\"msg\":\"" + m_text + "\""
            + "}";

    BLOGV("IME send event: %s\n", ime_info.c_str());
    m_opera.setStackAtTop(m_keyboardWindow);
    m_opera.sendPlatformEvent(m_keyboardWindow, ime_info);
    m_isShown = true;
}

void VirtualKeyboard::CommitEdit(std::vector<std::string> &arguments)
{
    BROWSER_LOG(INFO, "IME Commit: [%s][%s]",
                (arguments.size() >= 1) ? arguments[0].c_str() : "---",
                (arguments.size() >= 2) ? arguments[1].c_str() : "---");


    m_opera.setStackAtBottom(m_keyboardWindow);
    m_opera.hide(m_keyboardWindow);

    if (m_currentTarget.IsValid()) {
        m_opera.focus(m_currentTarget);
    }

    std::string submit = "false";
    if (arguments.size() >= 1)
        m_text = arguments[0];
    if (arguments.size() >= 2)
        submit = arguments[1];

    m_opera.commitIME(m_currentTarget, m_text, submit == "true");

    if ((true == keyboard_search_flag) && m_currentTarget.IsValid() && m_targetWindows[0] == m_currentTarget) {
        m_opera.sendKeyInput(opera::msg::KeyInput::KEYDOWN,
                             opera::msg::KeyInput::OMI_KEY_ENTER,
                             opera::msg::KeyInput::MODIFIER_NONE,
                             false,
                             "\n",
                             m_currentTarget);
        m_opera.sendKeyInput(opera::msg::KeyInput::KEYUP,
                             opera::msg::KeyInput::OMI_KEY_ENTER,
                             opera::msg::KeyInput::MODIFIER_NONE,
                             false,
                             "\n",
                             m_currentTarget);
    }
    if((true == isSpecialURL)&&m_currentTarget.IsValid()&&m_targetWindows[0]==m_currentTarget)
    {
        m_opera.sendKeyInput(opera::msg::KeyInput::KEYUP,
                 opera::msg::KeyInput::OMI_KEY_0,
                 opera::msg::KeyInput::MODIFIER_NONE,
                 false,
                 "\n",
                  m_currentTarget);
    }
    
    /* Bring up be OK directly next time*/
    // m_status = IME_READY;

    Json::Value o;
    Json::FastWriter writer;
    o["msg"] = "virtualKeyboardHide";
    m_opera.sendPlatformEvent(m_currentTarget, writer.write(o));

    m_isShown = false;
}

void VirtualKeyboard::CancleEdit()
{
    BROWSER_LOG(INFO, "Virtual Keyboard CancleEdit! ");

    m_opera.setStackAtBottom(m_keyboardWindow);
    m_opera.hide(m_keyboardWindow);
    //m_opera.cancelIME(m_window);

    if (m_currentTarget.IsValid()) {
        m_opera.focus(m_currentTarget);
    }
    m_isShown = false;

    /* Bring up by OK next time */
    //   m_status = IME_READY;

}

void VirtualKeyboard::OnPlatformMessage(opera::evt::PlatformMessage platform_msg)
{
    std::string command;
    std::vector<std::string> arguments;
    ParseCommand(platform_msg.message, command, arguments);
    BLOGV("Recv message from web: %s", (command.empty()) ? "Empty Command" : command.c_str());

    if (command == "commit-ime") {
        CommitEdit(arguments);
    } else if (command == "cancel-ime") {
        CancleEdit();
    } else if (command == "disableVKB") {
        BROWSER_LOG(INFO, "disable VKB\n");
        m_enable_vkb = false;
    } else if (command == "enableVKB") {
        BROWSER_LOG(INFO, "enable VKB\n");
        m_enable_vkb = true;
    }
}

void VirtualKeyboard::CreateUI()
{
#define VIRTUAL_KEYBOARD_URL  "VIRTUAL_KEYBOARD_URL"
    std::string url;
#if 0
    std::string resolution = browser().getHisenseOpera().getPref(m_window, "content_resolution");
    if (resolution == ZERO_RESOLUTION) {
        resolution = FHD;
    }
    BROWSER_LOG(ALWAYS, "resolution : %s", resolution.c_str());
#endif

    //FIXME: get this from configuration configuration:
    url = getenv(VIRTUAL_KEYBOARD_URL);
    if (url.empty() || url.length() < 5) {
        url = OperaDirFileURL("browser_ui/imeui.html");
    } else {
        //FIXME: get this from configuration:
        url += "?type=0";
        SysInfo* sysinfo = SysInfo::getSysInfoInstance();
        url += "&market=";
        url += sysinfo->getModel();
        url += "&area=";
        url += sysinfo->getRegion();
    }

    BLOGV("Virtual Keyboard URL: %s", url.c_str());
    m_startUrl = url;
    int width = browser().getGraphics().getWidth();
    int height = browser().getGraphics().getHeight();

    BrowserRect_t rect = {0, 0, width, height};
#if 0 //def WINDOW_TRANS
    m_keyboardWindow = m_opera.createWindow(true, false, false, rect, m_window);
#else
    m_keyboardWindow = m_opera.createWindow(false, false, rect, m_currentTarget);
#endif
    m_opera.disableScreenReaderHighlight(m_keyboardWindow);
    m_opera.setPref(m_keyboardWindow, "transparent", "true");
    m_opera.setStackAtBottom(m_keyboardWindow);
    m_opera.loadURL(m_keyboardWindow, url);
    m_opera.hide(m_keyboardWindow);

}

bool VirtualKeyboard::sendKeyInput(const opera::msg::KeyInput& key_input)
{

    m_opera.sendKeyInput(key_input.event_type,
                         key_input.virtual_key,
                         key_input.key_modifiers,
                         key_input.key_repeat,
                         key_input.key_value,
                         m_keyboardWindow);
}