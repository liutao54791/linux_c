#ifndef  MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/ipc.h>

#include <sys/msg.h>

using namespace std;

struct msg_st  
{  
    long int msg_type;
    std::string message;
};

int getMessage(key_t key,int msgflag);

bool recMessage(int msgid,void* receiveMessage,int size,int receiveMsgType,int msgflag);

bool sendMessage(int msgid,void* sendMessage,int size,int sendMessageType);

bool ctrlMessage(int msgid, int msgflag);

#endif