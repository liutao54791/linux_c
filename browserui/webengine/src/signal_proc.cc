#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <execinfo.h>

#include "signal_proc.h"
#include "browser_debug.h"
#include "browser-impl.h"

BrowserSignal* BrowserSignal::m_browserSignal = NULL;

BrowserSignal::BrowserSignal( BrowserImpl* browserImpl )
    : BrowserComponent( browserImpl )
{
    BROWSER_LOG(INFO, "");
    m_browserSignal = this;
}

BrowserSignal::~BrowserSignal()
{
    BROWSER_LOG(INFO, "");
}

//Kill -6 signal
void BrowserSignal::close_signal(int signal, siginfo_t *info, void *context)
{
    BROWSER_LOG(ERROR, "Close signal: %d", signal);
    //if (m_browserSignal && m_browserSignal->m_browserImpl)
       //m_browserSignal->browser().stop();
}

void BrowserSignal::abort_signal(int signal, siginfo_t *info, void *context)
{
    BROWSER_LOG(ERROR, "Abort signal: %d", signal);
    if (m_browserSignal )
        m_browserSignal->browser().stop();
}

void BrowserSignal::segv_signal(int signal, siginfo_t *info, void *context)
{
    static const char *si_code_strings[3] = {"", "SEGV_MAPERR", "SEGV_ACCERR"};
    static int cntr = 0;

    if (cntr > 5) {
        return;
    }
    cntr++;

    BROWSER_LOG(ERROR, "Segmentation fault:");
    BROWSER_LOG(ERROR, "info.si_signo: %d", signal);
    BROWSER_LOG(ERROR, "info.si_errno: %d", info->si_errno);
    BROWSER_LOG(ERROR, "info.si_code: %d (%s)", info->si_code, si_code_strings[info->si_code]);
    BROWSER_LOG(ERROR, "info.si_addr: 0x%x", (unsigned int)info->si_addr);

#define SEGV_HANDLER_WITH_TRACEBACK
#ifdef SEGV_HANDLER_WITH_TRACEBACK
#define SIZE 100
    void *buffer[SIZE];
    char **strings;

    int nptrs = backtrace(buffer, SIZE);
    BROWSER_LOG(ERROR, "==================================================================");
    BROWSER_LOG(ERROR, "Backtrace (%d addresses):", nptrs);

    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        BROWSER_LOG(ERROR, "Failed making backtrace.");
    } else {
        nptrs -= 2;
        for (int j = 0; j < nptrs; j++) {
            BROWSER_LOG(ERROR, "[%d] %s", j, strings[j]);
        }
        BROWSER_LOG(ERROR, "==================================================================");
        free(strings);
    }
#endif

    abort();
    usleep(5000000);
}

BrowserStatus BrowserSignal::start()
{
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO;

    action.sa_sigaction = close_signal;
    //sigaction(SIGHUP, &action, NULL);
    //sigaction(SIGTERM, &action, NULL);
    //sigaction(SIGINT, &action, NULL);
    sigaction(SIGKILL, &action, NULL);
    //sigaction(SIGQUIT, &action, NULL);

   // action.sa_sigaction = abort_signal;
   // sigaction(SIGABRT, &action, NULL);

  //  action.sa_sigaction = segv_signal;
   // sigaction(SIGSEGV, &action, NULL);

    return BrowserOK;
}


BrowserStatus BrowserSignal::stop()
{
    BROWSER_LOG( INFO, "" );
    return BrowserOK;
}

