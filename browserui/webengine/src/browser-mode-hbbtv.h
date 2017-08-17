#ifndef BROWSER_MODE_HBBTV_H
#define BROWSER_MODE_HBBTV_H

#include "omi_components/async_input_dispatcher_impl.h"
//#include <opera/events/LoadingProgress.h>
#include <opera/events/LoadingFinished.h>
#include <opera/events/LoadingStarted.h>


#include "browser-mode.h"
#include "hbbtvomi_components/hbbtv_controller.h"
#include "hbbtvomi_components/hbbtv/hbbtv_integration.h"

#include <opera/events/ParentalControlQueryEvent.h>

#include <opera/events/AITParsingStatus.h>
#include <opera/events/AITUpdated.h>
//#include <opera/events/AuthenticationRequest.h>
//#include <opera/events/BeforeURLRequest.h>
//#include <opera/events/BrowserGPUMemoryEvent.h>
//#include <opera/events/BrowserMemoryEvent.h>
//#include <opera/events/CancelActiveAndPendingDialogs.h>
#include <opera/events/CertificateError.h>
//#include <opera/events/CICAMFileRequested.h>
//#include <opera/events/ClientCertificateSelectEvent.h>
//#include <opera/events/DisplaySize.h>
//#include <opera/events/DSMCCFileRequested.h>
//#include <opera/events/DSMCCIsSameCarousel.h>
//#include <opera/events/DSMCCSubscribeStreamEvent.h>
//#include <opera/events/DSMCCUnsubscribeAllStreamEvents.h>
//#include <opera/events/DSMCCUnsubscribeStreamEvent.h>
//#include <opera/events/ExitingFullscreenMode.h>
//#include <opera/events/FullscreenModeRequest.h>
#include <opera/events/HbbTVApplicationBlocked.h>
#include <opera/events/HbbTVApplicationLoadTimedOut.h>
#include <opera/events/HbbTVApplicationNotStarted.h>
#include <opera/events/HbbTVApplicationStarted.h>
#include <opera/events/HbbTVApplicationStopped.h>
#include <opera/events/HbbTVApplicationVisibilityChanged.h>
#include <opera/events/HbbTVBroadcastRelatedStatusChanged.h>
#include <opera/events/HbbTVKeysetChanged.h>
//#include <opera/events/IMECancelled.h>
//#include <opera/events/IMEStart.h>
#include <opera/events/LoadingBlocked.h>
#include <opera/events/LoadingFinished.h>
#include <opera/events/LoadingStarted.h>
//#include <opera/events/MimeTypeRetrieved.h>
//#include <opera/events/NewWindowBlocked.h>
//#include <opera/events/NewWindow.h>
//#include <opera/events/OIPFApplicationActivated.h>
//#include <opera/events/OIPFApplicationDeactivated.h>
//#include <opera/events/OIPFApplicationStarted.h>
//#include <opera/events/OIPFApplicationStartFailed.h>
//#include <opera/events/OIPFApplicationStopped.h>
//#include <opera/events/PlatformMessage.h>
//#include <opera/events/RemoteXmlAITDownloadStatus.h>
//#include <opera/events/RendererProcessDied.h>
//#include <opera/events/RenderingBufferUpdate.h>
//#include <opera/events/RequestQuotaPermission.h>
//#include <opera/events/ResetDialogState.h>
//#include <opera/events/RunBeforeUnloadDialog.h>
//#include <opera/events/RunJavaScriptDialog.h>
//#include <opera/events/ScriptRequestCloseWindow.h>
//#include <opera/events/ScriptRequestWindowChangeBounds.h>
//#include <opera/events/TVStoreWindowClosed.h>
//#include <opera/events/UnhandledKeyInput.h>
//#include <opera/events/URLChanged.h>
//#include <opera/events/VisibleSSLStateChanged.h>

class BrowserModeHBBTV : public BrowserMode, public browser::hbbtv::HBBTVDelegate
{
public:
    BrowserModeHBBTV(BrowserImpl* browserImpl);

    virtual ~BrowserModeHBBTV();

    virtual BrowserStatus start();
    virtual void stop();
    virtual void enter(MultiModeArgument *argument = NULL);
    virtual void leave();

    virtual void pause() override;
    virtual void resume() override;
    virtual void injectXmlAit(std::string url) override;
    virtual void setNetStatus(const int status) override;
    virtual void ReTune();

    virtual void setUrl(std::string url);

    static bool InKeyset(opera::msg::KeyInput::Key virtual_key, const int keyset);
    int GetCurrentKeyset();

    virtual void processOOM() { }
    void onTTXKeyPressed();

private://Opera Event handlers:
    void registerEventListers();
    void OnAITParsingStatus(opera::evt::AITParsingStatus);
    void OnAITUpdated(opera::evt::AITUpdated);
    //void  OnAuthenticationRequest(opera::evt::AuthenticationRequest);
    //void  OnBeforeURLRequest(opera::evt::BeforeURLRequest);
    //void  OnBrowserGPUMemoryEvent(opera::evt::BrowserGPUMemoryEvent);
    //void  OnBrowserMemoryEvent(opera::evt::BrowserMemoryEvent);
    //void  OnCancelActiveAndPendingDialogs(opera::evt::CancelActiveAndPendingDialogs);
    void OnCertificateError(opera::evt::CertificateError);
    //void  OnCICAMFileRequested(opera::evt::CICAMFileRequested);
    //void  OnClientCertificateSelectEvent(opera::evt::ClientCertificateSelectEvent);
    //void  OnDisplaySize(opera::evt::DisplaySize);
    //void  OnDSMCCFileRequested(opera::evt::DSMCCFileRequested);
    //void  OnDSMCCIsSameCarousel(opera::evt::DSMCCIsSameCarousel);
    //void  OnDSMCCSubscribeStreamEvent(opera::evt::DSMCCSubscribeStreamEvent);
    //void  OnDSMCCUnsubscribeAllStreamEvents(opera::evt::DSMCCUnsubscribeAllStreamEvents);
    //void  OnDSMCCUnsubscribeStreamEvent(opera::evt::DSMCCUnsubscribeStreamEvent);
    //void  OnExitingFullscreenMode(opera::evt::ExitingFullscreenMode);
    //void  OnFullscreenModeRequest(opera::evt::FullscreenModeRequest);
    void OnHbbTVApplicationBlocked(opera::evt::HbbTVApplicationBlocked);
    void OnHbbTVApplicationLoadTimedOut(opera::evt::HbbTVApplicationLoadTimedOut);
    void OnHbbTVApplicationNotStarted(opera::evt::HbbTVApplicationNotStarted);
    void OnHbbTVApplicationStarted(opera::evt::HbbTVApplicationStarted);
    void OnHbbTVApplicationStopped(opera::evt::HbbTVApplicationStopped);
    void OnHbbTVApplicationVisibilityChanged(opera::evt::HbbTVApplicationVisibilityChanged);
    void OnHbbTVBroadcastRelatedStatusChanged(opera::evt::HbbTVBroadcastRelatedStatusChanged);
    void OnHbbTVKeysetChanged(opera::evt::HbbTVKeysetChanged);
    //void  OnIMECancelled(opera::evt::IMECancelled);
    //void  OnIMEStart(opera::evt::IMEStart);
    void OnLoadingBlocked(opera::evt::LoadingBlocked);
    void OnLoadingFinished(opera::evt::LoadingFinished);
    void OnLoadingStarted(opera::evt::LoadingStarted);
    //void  OnMimeTypeRetrieved(opera::evt::MimeTypeRetrieved);
    //void  OnNewWindowBlocked(opera::evt::NewWindowBlocked);
    //void  OnNewWindow(opera::evt::NewWindow);
    //void  OnOIPFApplicationActivated(opera::evt::OIPFApplicationActivated);
    //void  OnOIPFApplicationDeactivated(opera::evt::OIPFApplicationDeactivated);
    //void  OnOIPFApplicationStarted(opera::evt::OIPFApplicationStarted);
    //void  OnOIPFApplicationStartFailed(opera::evt::OIPFApplicationStartFailed);
    //void  OnOIPFApplicationStopped(opera::evt::OIPFApplicationStopped);
    //void  OnPlatformMessage(opera::evt::PlatformMessage);
    //void  OnRemoteXmlAITDownloadStatus(opera::evt::RemoteXmlAITDownloadStatus);
    //void  OnRendererProcessDied(opera::evt::RendererProcessDied);
    //void  OnRenderingBufferUpdate(opera::evt::RenderingBufferUpdate);
    //void  OnRequestQuotaPermission(opera::evt::RequestQuotaPermission);
    //void  OnResetDialogState(opera::evt::ResetDialogState);
    //void  OnRunBeforeUnloadDialog(opera::evt::RunBeforeUnloadDialog);
    //void  OnRunJavaScriptDialog(opera::evt::RunJavaScriptDialog);
    //void  OnScriptRequestCloseWindow(opera::evt::ScriptRequestCloseWindow);
    //void  OnScriptRequestWindowChangeBounds(opera::evt::ScriptRequestWindowChangeBounds);
    //void  OnTVStoreWindowClosed(opera::evt::TVStoreWindowClosed);
    //void  OnUnhandledKeyInput(opera::evt::UnhandledKeyInput);
    //void  OnURLChanged(opera::evt::URLChanged);
    //void  OnVisibleSSLStateChanged(opera::evt::VisibleSSLStateChanged);


private:

    virtual void updateKeySet(int) override;
    virtual void applicationStarted(const std::string & appurl) override;
    virtual void applicationStopped(const std::string & appurl) override;
    virtual int getCurrentBrowserMode() override;
    virtual opera::components::OperaController * getOperaController() override;
    virtual opera::Handle getCurrentHbbTVWindow()override;
    virtual void updateDTVTTXStatus(int status) override;
    virtual void onDTVStatusChanged(bool status) override;
private:
    void LoadBRApplication(const std::string &orgId, const std::string &appId);
    void updateMediaState();
    void setTVVideoWindowToFullScreenIfNeeded();
    void requestResumeDTVAsync();
private:

    class InputInterceptor
    : public opera::components::AsyncInputDispatcherImpl::InputEventsInterceptor
    {
    public:
        InputInterceptor(BrowserImpl* browserImpl, BrowserModeHBBTV* parent_hbbtv_mode);

        virtual bool WouldConsume(const opera::msg::KeyInput& key_input) OPERA_OVERRIDE;
        virtual bool InterceptKeyEvent(const opera::msg::KeyInput& key_input) OPERA_OVERRIDE;

    private:
        InputInterceptor(InputInterceptor&);
        InputInterceptor& operator=(InputInterceptor&);

        BrowserImpl & browser() const
        {
            return * m_browserImpl;
        }
        BrowserImpl* m_browserImpl;
        BrowserModeHBBTV* parent_hbbtv_mode_;
    };

    //void onLoadingProgress( opera::evt::LoadingProgress event);
    //void loadingStarted(opera::evt::LoadingStarted event);
    //void loadingFinished(opera::evt::LoadingFinished event);
    opera::Handle CreateHbbTVWindow(
                                    std::vector<opera::msg::CreateHbbTVWindow::PlatformProfile> platform_profiles);

    opera::Handle hbbtv_window_;
    std::unique_ptr<browser::hbbtv::HbbTVController> hbbtv_controller_ptr_;
    std::unique_ptr<browser::hbbtv::HbbTVIntegration> m_HbbTV; //Will replace hbbtvcontroller
    InputInterceptor* m_input_interceptor;

    void OverrideFallbackFonts();

    void ParentalControlQuery(opera::evt::ParentalControlQueryEvent event);

    int ParseParentalControlRating(const std::string& query_data);

    int GetSystemParentalRating();

    bool isAllowtoPlayback(int control_rating, int systrem_rating);
};

#endif
