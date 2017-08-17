#include "socket-manager.h"
#include "browser_debug.h"
#include "browser-impl.h"
#include "async_browser_task.h"
#include "task_manager.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include "base/log/browser_log.h"
#include "NotImplemented.h"
#define SERVER_NAME "/tmp/UNIX.domain"
#define BUFFER_SIZE 1024

SocketManager::SocketManager(BrowserImpl * browser)
    : BrowserComponent(browser)
    ,m_thread_running(false)
{
    BROWSER_LOG(INFO, "SocketManager()");
}

SocketManager::~SocketManager() {
    shutdown(m_socket_fd, 2);
    close(m_socket_fd);
    unlink(SERVER_NAME);
    BROWSER_LOG(INFO, "~SocketManager()");
}

BrowserStatus SocketManager::start() {
    BrowserStatus result = BrowserError;

    result = server_socket_start();
    if (result != BrowserOK) {
        return result;
    }

    if (pthread_create(&m_threadID, NULL, &socket_read, this) != 0) {
        BROWSER_LOG(ERROR, "Failed to create thread.");
        result = BrowserError;
        return result;
    }

    while(!m_thread_running)
        usleep(1000);

    BROWSER_LOG(INFO, "Success to create thread.");

    return result;
}

void SocketManager::stop() {
    BROWSER_LOG(INFO, "stop()");

    if (m_thread_running)
        m_thread_running = false;
    else {
        BROWSER_LOG( WARNING, "The thread is already stoped" );
        return;
    }

    shutdown(m_socket_fd, 2);
    close(m_socket_fd);
    unlink(SERVER_NAME);

    int result;
    if ( ( result = pthread_join(m_threadID, NULL) ) == 0 )
        BROWSER_LOG( ALWAYS, "Released the resource of the thread" );
    else
        BROWSER_LOG( ERROR, "Failed to release the resource of the thread, result : %d", result );
}

//static
void* SocketManager::socket_read(void* arg) {
    prctl( PR_SET_NAME, "SocketListener" );
    BROWSER_LOG( ALWAYS, "Socket listener Thread ID : %ld", syscall( __NR_gettid ) );

    SocketManager* socket_manager = static_cast<SocketManager*>(arg);

    socklen_t len = sizeof(socket_manager->m_client_addr);
    int client_fd;
    char receive_buffer[BUFFER_SIZE];

    if (!socket_manager->m_thread_running)
        socket_manager->m_thread_running = true;

    while(socket_manager->m_thread_running) {
        client_fd = accept(socket_manager->m_socket_fd, (struct sockaddr *)&(socket_manager->m_client_addr), &len);
        if (client_fd < 0) {
            BROWSER_LOG(ERROR, "Failed to accept specified socket error %d: %s", errno, strerror(errno));
            unlink(SERVER_NAME);
            break;
        }

        memset(receive_buffer, 0, BUFFER_SIZE);

        int num = read(client_fd, receive_buffer, BUFFER_SIZE);

        BROWSER_LOG(INFO, "message from client (number %d) : %s", num, receive_buffer);
        if (strcasecmp(receive_buffer, "stop")){
            socket_manager->browser().getTaskManager().postTask( hisense::AsyncBrowserTask::create(&socket_manager->browser(),"resume"));
        }else{
            socket_manager->browser().getTaskManager().postTask( hisense::AsyncBrowserTask::create(&socket_manager->browser(),"pause"));
        }
        NotImplemented();

    }
    close(client_fd);
    BROWSER_LOG(ALWAYS, "");
}


BrowserStatus SocketManager::server_socket_start() {
    unlink(SERVER_NAME);

    m_socket_fd = socket(PF_UNIX, SOCK_STREAM, 0);
    if (m_socket_fd < 0) {
        BROWSER_LOG(ERROR, "Failed to create server socket error %d: %s", errno, strerror(errno));
        return BrowserError;
    }

    m_server_addr.sun_family = AF_UNIX;
    strcpy(m_server_addr.sun_path, SERVER_NAME);

    //bind socket
    if (bind(m_socket_fd, (struct sockaddr *)&m_server_addr, sizeof(m_server_addr)) == -1) {
        BROWSER_LOG(ERROR, "Failed to bind specified address error %d: %s", errno, strerror(errno));
        return BrowserError;
    }

    //listen
    if (listen(m_socket_fd, 5) == -1) {
        BROWSER_LOG(ERROR, "Failed to listen specified address error %d: %s", errno, strerror(errno));
        return BrowserError;
    }

    //set socket no block
    //int flags = fcntl(m_socket_fd, F_GETFL, 0);
    //fcntl(m_socket_fd, F_SETFL, flags | O_NONBLOCK);

    return BrowserOK;
}
