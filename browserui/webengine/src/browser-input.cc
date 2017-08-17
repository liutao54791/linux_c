#include <directfb.h>

#include <opera/components/input/input_reader.h>

#include <opera/components/opera_controller.h>
#include <opera/events/DisplaySize.h>

#include "browser-input.h"
#include "omi_components/input/input_reader_apm.h"
#include "omi_components/input/input_reader_dfb_mouseonly.h"
#include "omi_components/input/input_reader_wrapper.h"
#include "browser-opera.h"
#include "browser-impl.h"
#include "browser_debug.h"
#include "graphics.h"
#include "NotImplemented.h"

#include "omi_components/async_input_dispatcher_impl.h"

BrowserInput::BrowserInput( BrowserImpl* browser)
: BrowserComponent(browser)
, m_input_reader_wrapper(NULL)
, m_dfb_mouse_input(NULL)
, m_apm_key_input(NULL)
, m_key_input_enabled(false)
, m_mouse_input_enabled(false)
, m_keyset(0)
{
    BROWSER_LOG( INFO, "" );
}

BrowserInput::~BrowserInput()
{
    BROWSER_LOG( INFO, "" );
}

hisense::input::InputReaderDFBMouseOnly * BrowserInput::CreateDFBMouseInput()
{
    //FIXME: We assume the Graphic module is started already.
    //Need to remove the dependency:
    Graphics & graphic = browser().getGraphics();
    IDirectFB* dfb = graphic.getDFB();
    if (!dfb) {
        BLOGE("The DFB instance is not Created yet.");
        return NULL;
    }
    hisense::input::InputReaderDFBMouseOnly *reader = new hisense::input::InputReaderDFBMouseOnly(dfb);
    if (!reader->Initialize()) {
        delete reader;
        reader = NULL;
    }
    return reader;
}

hisense::input::InputReaderAPM* BrowserInput::CreateAPMKeyInput()
{
    return new hisense::input::InputReaderAPM();
}

BrowserStatus BrowserInput::start()
{
    if ( m_input_helper.get() == NULL ) {
        HisenseOpera& hisenseOpera = browser().getHisenseOpera();

        const opera::Opera& opera = hisenseOpera.getOpera();
        //FIXME: We assume that setInputReader() is already called before start():
        if(m_input_reader_wrapper){
            BLOGW("BrowserInput module can't start twice");
           return BrowserError;
        }

        //Create the Main Input Reader
        m_input_reader_wrapper = new opera::components::input::InputReaderWrapper();
        m_input_reader_wrapper->Initialize();

       //Create Sub Input Readers:
        m_apm_key_input = CreateAPMKeyInput();// new hisense::input::InputReaderAPM();
        m_dfb_mouse_input = CreateDFBMouseInput();

        m_input_reader_wrapper->addInputReader("apm_key",m_apm_key_input);
        m_input_reader_wrapper->addInputReader("dfb_mouse",m_dfb_mouse_input);


        m_input_helper.reset( new opera::components::AsyncInputDispatcherImpl( opera, m_input_reader_wrapper ) );
#if 0
         //There are chances that the opera failed to send DisplaySize event to the input helper
         //So we send the default value here(Better than nothing):
        //FIXME: We assume that the graphic is started befor browserinput:
        Graphics& graphic = browser().getGraphics();
        int w = graphic.getWidth();
        int h = graphic.getHeight();
        m_input_helper->HandleDisplaySize(w,h);
#endif
        m_input_helper->Attach( hisenseOpera.getOperaController() );
    }

    m_input_helper->Start();

    return BrowserOK;
}
 void BrowserInput::resetMousePositon(int x, int y)
 {
     if(m_dfb_mouse_input){
         m_dfb_mouse_input->resetPositon(x,y);
     }
 }
 void BrowserInput::getMousePositon(int *x, int *y)
 {
     if(m_dfb_mouse_input){
         m_dfb_mouse_input->getPositon(x,y);
     }
 }
 void BrowserInput::updateDisplaySize(int w, int h)
{


    if (m_input_helper) {
        BLOGI("Update Input Display size to: (%d,%d)", w, h);
       m_input_helper->HandleDisplaySize(w, h);
    }
 }

BrowserStatus BrowserInput::stop()
{
    if (m_input_helper.get())
        m_input_helper->Stop();
    return BrowserOK;
}

void BrowserInput::setEventsIntercept(
    opera::components::AsyncInputDispatcherImpl::InputEventsInterceptor* events_interceptor ) {
    if (m_input_helper.get())
        m_input_helper->SetEventsInterceptor( events_interceptor );
}

void BrowserInput::enableMouseInput( bool enable ) {
    if (m_input_helper.get())
        m_input_helper->EnableMouseInput( enable );
    IDirectFBDisplayLayer* layer = browser().getGraphics().getLayer();
    if ( layer ) {
        if ( enable )
            BROWSER_LOG( INFO, "Enable DirectFB cursor" );
        else
            BROWSER_LOG( INFO, "Disable DirectFB cursor" );
        layer->EnableCursor( layer, enable );
    }
    m_mouse_input_enabled = enable;
}

void BrowserInput::enableKeyInput( bool enable ) {
    if (m_input_helper.get())
        m_input_helper->EnableKeyInput( enable );
    m_key_input_enabled = enable;
}

// void BrowserInput::setInputReader(opera::components::input::InputReader* reader)
// {
//     if(m_input_reader){
//         BROWSER_LOG(WARNING,"Input Reader Already set");
//         return ;
//     }
//     m_input_reader = reader;
// }

int BrowserInput::getCurrentKeyset()
{
    BrowserModeType mode = browser().currentMode();
    int keyset = 0;

    //webengine is not working:
    if (mode == BROWSER_MODE_OFF) {

        keyset = 0;
    } else if (mode == BROWSER_MODE_HBBTV) {
        //HBBTV Mode may ignore some of the key events:
        keyset = m_keyset;
    } else {
        //Other modes just intercept all the events:
        keyset = 0xFFFF;
    }

    //disable all keys if key input is disabled:
    if (!m_key_input_enabled) {
        keyset = 0;
    }
    //Enable navigation keys if mouse is enabled:
    if (m_mouse_input_enabled) {
        keyset |= m_apm_key_input->getKeysetBitForKey(opera::msg::KeyInput::OMI_KEY_ENTER);
    }

    return keyset;
}
void BrowserInput::updateKeySet(BrowserModeType mode,int set)
{
    m_keyset = set;
    browser().keySetChanged(mode,set);
}
int BrowserInput::generateKeyEvent(unsigned type,
                                 unsigned symbol,
                                 unsigned id,
                                 unsigned modifiers)
{
    if (!m_apm_key_input) return -1;

    int keyset = getCurrentKeyset();
    //We need to check if the key event is accepted before
    //we dispatch it, the APM need this.
    if (!m_apm_key_input->isIncludedInKeySet(keyset, type, symbol, id, modifiers) &&
            !m_apm_key_input->isAlwaysAcceptedKey(type, symbol, id, modifiers)) {
        BLOGI("Ignore APM Key Event,keyset= %d, symb=%d", keyset, symbol);
        return -2;
    }

    opera::msg::KeyInput key_input;
    BLOGV("Generate Key Event: type=%d,symbol=0X%x,id=%d,modifier=%d", type,symbol,id,modifiers);

    m_apm_key_input->generateKeyEvent(type,symbol,id,modifiers);
    return 0;
}

