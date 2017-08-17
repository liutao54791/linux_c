#ifndef BROWSER_SIGNAL_PROC
#define BROWSER_SIGNAL_PROC

#include <signal.h>

#include "browser_type.h"

#include "browser_component.h"

class BrowserSignal : public BrowserComponent
{
public:
    BrowserSignal(BrowserImpl* browserImpl);
    ~BrowserSignal();

    BrowserStatus start();
    BrowserStatus stop();

    static void segv_signal(int signal, siginfo_t *info, void *context);
    static void abort_signal(int signal, siginfo_t *info, void *context);
    static void close_signal(int signal, siginfo_t *info, void *context);

private:

private:
    static BrowserSignal* m_browserSignal;
};

#endif
