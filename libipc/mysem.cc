
#include "mysem.h"

void release_sem(int semid,int WhitchSemToRelease)
{
    struct sembuf sops;

    sops.sem_num = WhitchSemToRelease;
    sops.sem_op = +1;
    semop(semid,&sops,1);
}

void get_sem(int semid, int WhitchSemToGet)
{
    struct sembuf sops;

    sops.sem_num = WhitchSemToGet;
    sops.sem_op = -1;
    semop(semid,&sops,1);
}

int open_sem(key_t key, int nsem)
{
    int semid;
    if (semid = semget(key,nsem,IPC_CREAT) < 0)
    {
    	perror("sem open error");
    	return -1;
    }
    return semid;
}

bool ctrl_sem(int semid,int WhitchSemToSet,int tosetval)
{
    if (semctl(semid,WhitchSemToSet,SETVAL,tosetval) < 0)
    {
        perror("semctl error");
        return false;
    }
    return true;
}

