
#include "myshm.h"

int get_shm(key_t key, int size)
{    
    int shmid;
    if ((shmid = shmget(key, size,IPC_CREAT)) < 0)
    {
        perror("shmget error");
        return -1;
    }
    return shmid;
}

bool write_shm(int shmid, char* writebuf)
{
    char* shm;
    if ((shm = (char*)shmat(shmid, 0, 0))  < 0 )
    {
         perror("shmat error");
         return false;
    }
    strcpy(shm, writebuf);
    printf("shm is %s\n", shm);
    if (shmdt(shm) < 0)
    {
         perror("shmdt error");
         return false;
    }
    return true;
}

bool read_shm(int shmid, char* readbuf)
{
    char* shm;
    if ((shm = (char*)shmat(shmid, 0, 0)) < 0 )
    {
         perror("shmat error");
         return false;
    }
    strcpy(readbuf, shm);
    printf("shm is %s\n", shm);
    if (shmdt(shm) < 0)
    {
         perror("shmdt error");
         return false;
    }
    return true;
}




