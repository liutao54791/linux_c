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

/*msgtyp 0 接收第一个消息*/
/*msgtyp <0 接收类型等于或者小于msgtyp绝对值的第一个消息*/
/*msgtyp >0 接收类型等于msgtyp的第一个消息*/
/*msgflg 0 阻塞式接收消息，没有该类型的消息msgrcv函数一直阻塞等待*/
/*msgflg IPC_NOWAIT 如果没有返回条件的消息调用立即返回，此时错误码为ENOMSG*/
/*msgflg IPC_EXCEPT 与msgtype配合使用返回队列中第一个类型不为msgtype的消息*/
/*msgflg IPC_NOERROR 如果队列中满足条件的消息内容大于所请求的size字节，则把该消息截断，截断部分将被丢弃*/


struct msg_st  
{  
    long int msg_type;
    char  message[512];
};

int getMessage(key_t key,int msgflag);

bool recMessage(int msgid,struct msg_st* receiveMessage,size_t size,long int receiveMsgType,int msgflag);

bool sendMessage(int msgid,struct msg_st* sendMessage,size_t size,int msgflag);

bool ctrlMessage(int msgid, int msgflag);

#endif