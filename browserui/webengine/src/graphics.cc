#include <opera/components/opera_controller.h>
#include <opera/components/input/linux_input.h>

#include "graphics.h"
#include "browser-opera.h"
#include "configuration.h"
#include "browser-input.h"
#include "browser-impl.h"
#include "browser_debug.h"

extern "C"{
    void opera_set_dfb_handle(IDirectFB *handle);
}

Graphics::Graphics( BrowserImpl* browserImpl )
    : BrowserComponent( browserImpl )
    , m_width( 0 )
    , m_height( 0 )
    , m_Window( NULL )
{
    BROWSER_LOG(INFO, "");
}

Graphics::~Graphics()
{
    if(m_Window){
        m_Window->Release(m_Window);
        m_Window = NULL;
    }
    BROWSER_LOG(INFO, "");
}

BrowserStatus Graphics::start()
{
    opera::components::CommandLine& commandLine = browser().getConfiguration().getCommandLine();
    m_config.backend = commandLine.GetGraphicsBackend();


    std::string StrWidth = browser().getConfiguration().getValueByKey("width");
    std::string StrHeight = browser().getConfiguration().getValueByKey("height");

    int width;
    int height;
    if (!StrWidth.empty()) {
        width = atoi(StrWidth.c_str());
        m_width = width;
    }
    if (!StrHeight.empty()) {
        height = atoi(StrHeight.c_str());
        m_height = height;
    }

    BROWSER_LOG(INFO, "width : %d, height : %d", width, height);

// opera::components::input::InputReader* input_reader= NULL;

#if defined(USE_DIRECTFB)

    if ( !opera::components::directfb_utils::InitializeDirectFBComponents(
        m_config.backend, commandLine,
        m_dfb_instance, m_dfb_display,
        nullptr, //Input reader shouldn't be here, move to browser-input.
        width, height, &m_Window)) {
        BROWSER_LOG( ERROR, "Failed to init DirectFB component");
        return BrowserError;
    }
 //set the dfb input reader to browser input components:
//     browser().getBrowserInput().setInputReader(input_reader);
    // Required by the DirectFB graphics backend, ignored by
    // the other backends so we can always set it here.
    m_config.directfb = m_dfb_instance.directfb();
//    if(m_dfb_instance.directfb())
//    {
//        opera_set_dfb_handle(m_dfb_instance.directfb());
//    }
#endif  // defined(USE_DIRECTFB)
///////////////////////////////////////////////////////////////////////

    HisenseOpera& hisenseOpera = browser().getHisenseOpera();

    // Init Opera
    if ( hisenseOpera.initialize() != BrowserOK ) {
        BROWSER_LOG( ERROR, "Failed to init HisenseOpera" );
        return BrowserError;
    }

    BROWSER_LOG( INFO, "Succeed to init HisenseOpera");

#if defined(USE_DIRECTFB)
    // Start rendering to the DirectFB display using the OperaController
    if ( m_config.backend != opera::StaticConfiguration::OPENGLES ) {
        if ( hisenseOpera.getOperaController() ) {
            if ( !m_dfb_display.Attach( hisenseOpera.getOperaController() ) ) {
                BROWSER_LOG( ERROR, "Failed to Attach OperaController" );
                return BrowserError;
            }
        }
        else {
            BROWSER_LOG( ERROR, "OperaController is NULL" );
            return BrowserError;
        }
    }
#endif  // defined(USE_DIRECTFB)

    return BrowserOK;
}

BrowserStatus Graphics::stop()
{
    BROWSER_LOG( INFO, "" );
    return BrowserOK;
}

opera::StaticConfiguration& Graphics::getOperaConfiguration() {
    return m_config;
}

BrowserStatus Graphics::requestFocus( bool request, bool visible )
{
    BROWSER_LOG(INFO, "request: %d, visible: %d",request, visible);

    // reset display layer size
    if (request || visible) {
        IDirectFBDisplayLayer *layer = NULL;
        IDirectFB *dfb = (IDirectFB*)(m_config.directfb);
        if (dfb) {
            dfb->GetDisplayLayer(dfb, 0, &layer);
            DFBDisplayLayerConfig config;
            layer->GetConfiguration(layer, &config);
            if (layer
                && (config.width != m_width || config.height != m_height)) {
                DFBDisplayLayerConfig layer_config;
                layer->SetCooperativeLevel(layer, DLSCL_ADMINISTRATIVE);
                layer_config.flags = (DFBDisplayLayerConfigFlags)(DLCONF_WIDTH | DLCONF_HEIGHT);
                layer_config.width = m_width;
                layer_config.height = m_height;
                layer->SetConfiguration(layer, &layer_config);
                BROWSER_LOG(INFO, "adjust layer size form [ %d x %d ] to [ %d x %d ]",
                            config.width, config.height,
                            layer_config.width, layer_config.height);
                layer->GetConfiguration(layer, &config);
                BROWSER_LOG(INFO, "result is [ %d x %d ]",
                            config.width, config.height);
            }
        }
        if(m_Window){
            u8 currentOpacity;
            m_Window->GetOpacity(m_Window, &currentOpacity);
            if(currentOpacity!= 0xFF) {
                BROWSER_LOG(INFO, "SetOpacity  0xFF");
                m_Window->SetOpacity(m_Window, 0xFF);
            }
        }
    }

    if (m_config.backend != opera::StaticConfiguration::OPENGLES) {
        if (!m_dfb_display.requestFocus(request, visible)) {
            return BrowserError;
        }
    }

    return BrowserOK;
}

BrowserStatus Graphics::lostFocus()
{
    if(m_Window){
        u8 currentOpacity;
        m_Window->GetOpacity(m_Window, &currentOpacity);
        if(currentOpacity!= 0x00) {
            BROWSER_LOG(INFO, "SetOpacity  0x00");
            m_Window->SetOpacity(m_Window, 0x00);
        }
    }
    return BrowserOK;
}
