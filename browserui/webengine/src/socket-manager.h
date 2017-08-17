#ifndef _SOCKET_MANAGER_H_
#define _SOCKET_MANAGER_H_

#include "browser_type.h"
#include "browser_component.h"
#include <sys/un.h>
#include <pthread.h>

class SocketManager : public BrowserComponent
{
public:
    SocketManager(BrowserImpl* browserImpl);
    ~SocketManager();

    BrowserStatus start();
    void stop();

private:
    static void* socket_read(void* arg);

    BrowserStatus server_socket_start();

private:
    int m_socket_fd;

    struct sockaddr_un m_client_addr;
    struct sockaddr_un m_server_addr;

    pthread_t m_threadID;
    bool m_thread_running;
};

#endif
