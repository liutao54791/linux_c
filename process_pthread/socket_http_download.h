#ifndef SOCKET_HTTP_D_H
#define SOCKET_HTTP_D_H


#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/time.h>



struct resp_header//保持相应头信息
{
    int status_code;          //HTTP/1.1 '200' OK
    char content_type[128];   //Content-Type: application/gzip
    long content_length;      //Content-Length: 11683079
    char file_name[256];
};

struct resp_header resp;//全剧变量以便在多个进程中使用

void parse_url(char *url, char *domain, int *port, char *file_name);

struct resp_header get_resp_header(char *response);

void get_ip_addr(char *domain, char *ip_addr);

void progressBar(long cur_size, long total_size);

int download_main(char *m_url);

#endif


