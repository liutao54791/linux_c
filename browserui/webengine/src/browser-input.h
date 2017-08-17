#ifndef INPUT_READER_H
#define INPUT_READER_H

#include <memory>
#include "omi_components/async_input_dispatcher_impl.h"
#include "browser_component.h"
#include "browser_type.h"

namespace opera {
namespace components {
class OperaController;
namespace input {
class InputReader;
class InputReaderWrapper;
}//namespace input
}//namespace components
}//namespace opera

namespace hisense {
namespace input {
class InputReaderAPM;
class InputReaderDFBMouseOnly;
}
}
class APMInputReader;

class BrowserInput : public BrowserComponent
{
public:
    BrowserInput(BrowserImpl* browserImpl);
    ~BrowserInput();

    BrowserStatus initialize();

    BrowserStatus start();
    BrowserStatus stop();
    int generateKeyEvent(unsigned type,
                         unsigned symbol,
                         unsigned id,
                         unsigned modifiers
                         );
    void setEventsIntercept(
                            opera::components::AsyncInputDispatcherImpl::InputEventsInterceptor* events_interceptor);
    void enableMouseInput(bool enable);
    void enableKeyInput(bool enable);
    //The key set is used to determing if the key is accepted before dispatch.
    void updateKeySet(BrowserModeType mode,int keyset);
    int getCurrentKeyset();
    void updateDisplaySize(int w, int h);
    void resetMousePositon(int x, int y);
    void getMousePositon(int *x, int *y);
    //    void setInputReader(opera::components::input::InputReader*);
private:
    hisense::input::InputReaderAPM* CreateAPMKeyInput();
    hisense::input::InputReaderDFBMouseOnly * CreateDFBMouseInput();
private:
    opera::components::input::InputReaderWrapper * m_input_reader_wrapper; //main input reader
    hisense::input::InputReaderDFBMouseOnly* m_dfb_mouse_input; // mouse only
    hisense::input::InputReaderAPM* m_apm_key_input; // key from apm
    std::shared_ptr<opera::components::AsyncInputDispatcherImpl> m_input_helper;
    bool m_key_input_enabled;
    bool m_mouse_input_enabled;
    unsigned short m_keyset;
};

#endif