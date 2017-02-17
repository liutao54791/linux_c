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

#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "person.h"
#include "son.h"
#include "grandson.h"

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

// 进程间通信-匿名管道，只适用于有血缘关系的进程
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
    pid_t pid;
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

//进程间通信-有名管道
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

//进程间通信-信号量互斥实验
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
    fd = open("/home/liutao/board.txt",O_RDWR|O_APPEND);

    //获取信号量
    get_sem(semid);
    /*向公告板文件写入 数学课*/
    write(fd,"class math",11);
    //暂停10s
    sleep(10);
    //公告板文件写入 取消
    write(fd,"is cancel",11);
    printf("val is %d\n", semctl(semid,0,GETVAL));
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
    fd = open("/home/liutao/board.txt",O_RDWR|O_APPEND);
    
    //获取信号量
    get_sem(semid);
    //2 写入英语课考试
    write(fd,"englishi exam",20);
    release_sem(semid);

   //3 关闭公告板
    close(fd);
}

//进程间通信-共享内存
void shm_share(void)
{
    pid_t pid;

    if ((pid = fork()) < 0 )
    {
         perror("fork error");
    }else if (pid == 0){
        int shmid;
        key_t key;
        char* shm;
        key = ftok("/home", 1);

        if ((shmid = shmget(key, 1024,IPC_CREAT)) < 0)
        {
            perror("shmget error");
            exit(1);
        }
        shm = (char*)shmat(shmid, 0, 0);
        strcpy(shm, "hello hisense");
        printf("shm is %s\n", shm);
        if (shmdt(shm) < 0)
        {
             perror("shmdt error");
             exit(1);
        }
    }else if (pid > 0)
    {
        int shmid;
        key_t key;
        char* shm;
        key = ftok("/home", 1);
        //char* buf;
        //buf = (char*)malloc(1024);
        sleep(2);
        if ((shmid = shmget(key, 1024,IPC_CREAT)) < 0)
        {
            perror("shmget error");
            exit(1);
        }
        shm = (char*)shmat(shmid, 0, 0);
        printf("memory is %s\n", shm);
        if (shmdt(shm) < 0)
        {
             perror("shmdt error");
             exit(1);
        }
        //printf("memory is %s\n", buf);
    }

}

//线程编程

void *pthread_func(void * arg)
{
    pid_t pid;
    int i = 0;

    pid = getpid();
    printf("the pid is %d\n", pid);
    while(i != 10)
    {
        printf("hello my thread\n");
        i ++;
    }
}

//程序设计中经常会用到的回调函数实例
void share_callback(int a)
{
    if (a == 5)
    {
        printf("you are coming\n");
    }

}


int main(int argc, char* argv[])
{
/*********************************linux_c 进程间通信实验********************************/
    if (argc == 2)
    {
        int fd;
        fd = open("/home/liutao/flag.txt",O_RDWR);
        if (fd < 0 )
        {
             perror("open error");
        }
    
        ftruncate(fd,0);
        lseek(fd, 0, SEEK_SET);
        write(fd, "0123456789",sizeof("0123456789"));
        close(fd);

    }else if (argc == 3)
    {
        FILE *fp1, *fp2;
        char fpbuf[1024];
        int nbytes;

        if ((fp1 = fopen("/home/liutao.ex/flag.txt", "rb")) == NULL)
        {
            perror("failed open fp1");
        }
        if ((fp2 = fopen("/home/liutao.ex/dest.txt", "wb")) == NULL)
        {
            perror("failed open fp2");
        }

        while((nbytes = fread(fpbuf, sizeof(char), 1024, fp1)) > 0)
        {
            if (fwrite(fpbuf, sizeof(char), nbytes, fp2) == -1)
            {
                perror("failed to write");
            }
        }
        printf("the fpbuf is %s\n", fpbuf);
        if (nbytes == -1)
        {
            perror("failed to read");
        }

        fclose(fp1);
        fclose(fp2);
    }else{
        struct Node
        {
            int a;
            int b;
            int c;
        };
        char  buf[] = "hello hisense";
        char* cpbuf;
        cpbuf = (char*)malloc(sizeof(buf));
        strcpy(cpbuf, buf);
        printf("the cpbuf is %s\n", cpbuf);
        free(cpbuf);
        Node* m_node = (Node*)malloc(sizeof(struct Node));
        m_node -> a = 10;
        m_node -> b = 11;
        m_node -> c = 12;
    
        int val = 2;
    
        while(val < 10)
        {
            val ++;
            std::cout << "val is " << val << std::endl;
        }
    
        std::cout << "struct val is " << m_node -> c << std::endl;
    
        free(m_node);

/***************************************c++实验***************************************/
        CPerson m_person;
        CSon m_son;

        CPerson* p = new CSon;
        CPerson* q = new GrandSon;
        p->SayTruth();
        q->SayTruth();
        m_person.set_my_age(15);
        m_person.set_my_salary(100);
        m_person.SayTruth();
    
        m_son.SonSetAll(10,100);
        m_son.OutPutName();
        m_son.callback(10, share_callback);
        m_son.SayTruth();


/***************************************线程实验***************************************/
        pthread_t thread_id;
        pid_t pid;
        int err;
    
        pid = getpid();
        printf("pid is %d\n", pid);
        if ((err = pthread_create(&thread_id,NULL, pthread_func, NULL)) != 0)
        {
            perror("pthread create failed");
        }

//主线程先休眠让创建的线程先执行
        sleep(1);
/***************************************文件编程实验***************************************/
        int fd1;
        char filebuf[255];
        int offset;
        fd1 = open("/home/liutao.ex/flag.txt",O_RDWR);
        if (fd1 < 0 )
        {
             perror("open error");
        }
    
        ftruncate(fd1,0);
        lseek(fd1, 0, SEEK_SET);
        write(fd1, "0123456789",sizeof("0123456789"));
        printf("sizeof is %d\n", sizeof("0123456789"));

        offset = lseek(fd1, 0 ,SEEK_CUR);
        printf("after write the offset is %d\n", offset);
        close(fd1);

        fd1 = open("/home/liutao.ex/flag.txt",O_RDWR);
        if (fd1 < 0 )
        {
             perror("open error");
        }

        offset = lseek(fd1, 0 ,SEEK_CUR);                          //得到当前的位置
        printf("before read the offset is %d\n", offset);

        offset = lseek(fd1, sizeof("0123456789")-5 ,SEEK_SET);     //设置位置
        printf("read count is %d\n", read(fd1,filebuf,sizeof(filebuf)));

        offset = lseek(fd1, 0 ,SEEK_CUR);                          //得到当前的位置
        printf("after read the offset is %d\n", offset);
        printf("filebuf is %s\n", filebuf);

        if (filebuf[0] == '0')
        {
            printf("the function is closed \n");
        }
        else{
            printf("the function is open\n");
        }

        close(fd1);
        char* pp = "123456789";
        std::string url;
        url = pp;
        printf("the url is %s\n" ,url.c_str());
        return 0;
        }
    
}
