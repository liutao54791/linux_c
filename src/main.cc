
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <sys/wait.h>

#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define MAX_STR_SIZE   256
#define BUFSZ          PIPE_BUF
static  char buf[BUFSZ];

char* basename(char* full_name)
{
    char* p;

    p = rindex(full_name,'/');

    if (p == '\0')
    {
        p = full_name;/* code */
    }
    else
    {
        p++;
    }
    return p;
}

void m_open_file(void)
{
    int fd;

    int errno;

    fd = open("/home/liutao/liu.txt",O_RDWR);

    if (errno == 0)
    {
        printf("open successful\n");
    }else
    {
        int err = errno;
        printf("Fail:the reason is %s\n",strerror(err));
    }
}

void m_malloc(int **p)
{
    *p = (int *)malloc(sizeof(int));
}

void m_getenv(int argc, char* argv[])
{
    char* env;

    char* p;

    env = getenv("HOME");

    if (env == NULL)
    {
        perror("failed to get env");
        exit(1);
    }

    printf("$HOME is %s\n",env);

    p = basename(argv[0]);

    printf("file name is %s\n",p );
}

void m_pid(void)
{
    pid_t pid;

    pid = fork();
    if (pid < 0)
    {
        perror("fork error");/* code */
    }else if (pid == 0)
     {
         pid = getpid();

         sleep(5);
         printf("the child process is done\n");
         exit(0);
         /* code */
     }else
      {
          if (wait(NULL) == -1)
          {
              perror("wait error");
              exit(1);
          }
          sleep(3);
          printf("the father process is over\n");
      }
}

// pipe
void my_pipe(void)
{
    int fd[2];

    char str[MAX_STR_SIZE];

    char* m_string = "this is pipe ,hello world!";

    if (pipe(fd) < 0)
    {
        perror("pipe error");
        exit(1);
    }

    printf("the sting is %s\n",m_string);
    if ( write(fd[1], m_string, sizeof(m_string)) == -1)
    {
        perror("write error");
        return;
    }
    close(fd[1]);

    sleep(5);

    if (read(fd[0], str, sizeof(m_string)) == -1)
    {
        perror("read error");
        return;
    }
       
    close(fd[0]);

    printf("the str is %s\n",str);

    printf("the pipe descriptors is %d %d\n",fd[0],fd[1]);

    

}

int m_brother_pid_pipe(void)
{
    int fd[2];
    pid_t pid;

    char str[MAX_STR_SIZE];

    if (pipe(fd) < 0)
    {
        perror("pipe error");
        exit(0);
        return 0;
    }

    char* m_string = "hello brother pid pipe\n";
    if ((pid = fork()) < 0)
    {
        perror("fork error");
        exit(0);
    }else if (pid == 0)
    {
        close(fd[0]);

        write(fd[1], m_string, strlen(m_string));
        printf("this is child one\n");
    }else
    {
        if ((pid = fork()) < 0)
        {
            perror("fork error");
            exit(0);
        }else if (pid == 0)
        {
            close(fd[1]);
            read(fd[0], str, strlen(m_string));

            printf("this is child two, str is %s\n",str );
        }
        else
        {
            close(fd[0]);
            close(fd[1]);
            //write(STDOUT_FILENO, str, strlen(m_string));
            printf("this is father\n");
            exit(0);
        }
    }
}

void m_father_child_pipe(void)
{
    pid_t pid ;
    int   fd[2];
    char* m_string = "hello child\n";
    char  str[MAX_STR_SIZE];

    if (pipe(fd) < 0 )
    {
        perror("pipe error");
        exit(1);
    }

    if ((pid = fork()) < 0)
    {
        perror("fork error");
        exit(1);
    }else if (pid == 0)
    {
        printf("this is child \n");

        close(fd[1]);
        read(fd[0], str, strlen(m_string));
        
        printf("str is %s\n", str);
        perror("read error");
        exit(1);

    }else if (pid > 0)
    {
        close(fd[0]);
        write(fd[1], m_string, strlen(m_string));
        printf("this is father\n");
    }
}



//fifo exersices
void m_creat_fifo(char* fifo_name)
{
    mode_t mode = 0666;
    if (mkfifo(fifo_name,mode) < 0)
    {
        perror("make fifo error");
        exit(1);
    }else {
        printf("the fifo name is %s\n", fifo_name);
    }
}

void m_read_fifo(char* fifo_name)
{
    int fd;
    int len;

    if((fd = open(fifo_name, O_RDONLY)) < 0)
    {
        perror("open fifo failed");
        exit(1);
    }

    while((len = read(fd, buf,BUFSZ)) > 0)
    {
        printf("read fifo is %s\n", buf);
    }

    close(fd);

}

void m_write_fifo(char* fifo_name)
{
    int     fd;
    int     n;
    time_t  tp;
    char    i;
    char    buf[BUFSZ];

    if ((fd = open(fifo_name, O_WRONLY)) < 0)
    {
        perror("open error");
        exit(1);
    }
    for ( i=0 ; i<10; i++)
    {
                            /*循环10次向FIFO中写入数据*/
        time(&tp);          /*取系统当前时间*/
                            /*使用sprintf 函数向buf中格式化写入进程ID 和时间值*/
        n = sprintf(buf,"write_fifo %d sends %s",getpid(),ctime(&tp));

        printf("Send msg:%s",buf);

        if((write(fd, buf, n+1)) < 0)
        {                    /*写入到FIFO中*/
            perror("write");
            close(fd);       /*关闭FIFO文件*/
        }
    }
}

//sem 信号量互斥实验
void release_sem(int semid)
{
    struct sembuf sops;

    sops.sem_num = 0;
    sops.sem_op = +1;
    semop(semid,&sops,1);
}

void get_sem(int semid)
{
    struct sembuf sops;

    sops.sem_num = 0;
    sops.sem_op = -1;
    semop(semid,&sops,1);
}

void sem_stua(void)
{
    int fd = 0;
    key_t key;
    int semid;
    
    key = ftok("/home",1);
    
    //创建\打开信号量
    semid = semget(key,1,IPC_CREAT);
    if (semctl(semid,0,SETVAL,1) < 0)
    {
        perror("semctl error");
        exit(1);
    }

    printf("val is %d\n", semctl(semid,0,GETVAL));
    //打开文件
    fd = open("/home/ronald/board.txt",O_RDWR|O_APPEND);

    //获取信号量
    get_sem(semid);
    /*向公告板文件写入 数学课*/
    write(fd,"class math",11);
    //暂停10s
    sleep(10);
    //公告板文件写入 取消
    write(fd,"is cancel",11);

    //释放信号量
    release_sem(semid);
    
    close(fd);
}

void sem_stub(void)
{
    int fd = 0;
    int semid;
    key_t key;
    
    key = ftok("/home",1);
    
    //创建\打开信号量集合
    semid = semget(key,1,IPC_CREAT);
    printf("val is %d\n", semctl(semid,0,GETVAL));
    //1 打开公告板
    fd = open("/home/ronald/board.txt",O_RDWR|O_APPEND);
    
    //获取信号量
    get_sem(semid);
    //2 写入英语课考试
    write(fd,"englishi exam",20);
    release_sem(semid);

   //3 关闭公告板
    close(fd);
}

//共享内存
int share(void)
{
    pid_t pid;

    //创建子进程
    pid = fork();
    if(pid == -1)
    {
        perror("error!!!!");
        return 1;
    }
    else if(pid == 0)
    {
        char *addr;
        int shmid;
        key_t key = ftok("/home", 1);
        //创建一个共享内存
        shmid = shmget(key,1024,IPC_CREAT|0660);
        if(shmid == -1)
        {
            perror("create share memory failed!");
            return 1;
        }
        
        //连接共享内存到一个进程的空间
        addr = (char*)shmat(shmid,NULL,0);
        if(addr == (char *)(-1))
        {
            perror("cannot attach!");
            return 1;
        }
        printf("share memory segment's address:%x\n",addr);
        
        //向这个空间中写入数据
        strcpy(addr,"welcome to linux!");
        printf("child process string is' %s'\n",addr);
        if(shmdt(addr)==-1)
        {
            perror("release failed!");
            return 1;
        }
        exit(0);
    }
    else
    {
        wait(NULL);
        char *addr;
        int shmid;
        key_t key = ftok("/home", 1);
        //创建一个共享内存
        shmid = shmget(key,1024,IPC_CREAT|0660);
        addr = (char*)shmat(shmid,NULL,0);
        if(addr == (char *)(-1))
        {
            perror("cannot attach!");
            return 1;
        }

        printf("parent process string is '%s'\n",addr);//父进程读取共享内存的数据
        
        //父进程中分离共享内存
        if(shmdt(addr)==-1)
        {
            perror("release failed!");
            return 1;
        }
        
        //删除共享内存
        if(shmctl(shmid,IPC_RMID,NULL)==-1)
        {
            perror("failed!");
            return 1;
        }
    }
    return 0;

}

int main(int argc, char* argv[])
{
    share();
    if (argc == 1)
    {
        sem_stua();
    }else
    {   
        sem_stub();
    }
    exit(1);
    return 0;
}


