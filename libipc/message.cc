
#include "message.h"

int getMessage(key_t key,int msgflag)
{
    int msgid;
    msgid = msgget(key, msgflag);  
    if (msgid == -1)
    {
        perror("get message error");
        return -1;
    }
    return msgid;
}

bool recMessage(int msgid,struct msg_st* receiveMessage,size_t size,long int receiveMsgType,int msgflag)
{
        if(msgrcv(msgid, (void*)receiveMessage, size, receiveMsgType, msgflag) < 0)
        {
            perror("msgrcv failed with errno\n");
            return  false;
        }
        return true;
}

bool sendMessage(int msgid,struct msg_st* sendMessage,size_t size,int msgflag)
{
        if(msgsnd(msgid, (void*)sendMessage, size, msgflag) == -1)
        {
            perror("send error");
            return false;
        }
        return true;
}

bool ctrlMessage(int msgid, int msgflag)
{
    if(msgctl(msgid, msgflag, 0) == -1)  
    {  
        perror("msgctl failed\n");
        return false;
    }
    return true;
}
