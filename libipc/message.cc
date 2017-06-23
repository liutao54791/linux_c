
#include "message.h"

int getMessage(key_t key,int msgflag)
{
    int msgid;
    msgid = msgget(key, msgflag);  
    if (msgid == -1)
    {
        return -1;
    }
    return msgid;
}

bool recMessage(int msgid,void* receiveMessage,int size,int receiveMsgType,int msgflag)
{
        if(msgrcv(msgid, (void*)&receiveMessage, size, receiveMsgType, msgflag) == -1)
        {
            fprintf(stderr, "msgrcv failed with errno: %d\n", errno);
            return  false;
        }
        return true;
}

bool sendMessage(int msgid,void* sendMessage,int size,int sendMessageType)
{
        if(msgsnd(msgid, (void*)&sendMessage, size, sendMessageType) == -1)
        {
            fprintf(stderr, "msgsnd failed\n");
            return false;
        }
        return true;
}


bool ctrlMessage(int msgid, int msgflag)
{
    if(msgctl(msgid, msgflag, 0) == -1)  
    {  
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        return false;
    }
    return true;
}