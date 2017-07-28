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

#include "compoent.h"
#include "person.h"
#include "son.h"
#include "grandson.h"
#include "libso.h"
#include "uart.h"
#include "m_pthread.h"
#include "socket_http_download.h"
#include "message.h"
#include "runprocess.h"
#include "mystl.h"

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
    if (argc < 1)
    {
        return 0;
    }
    printf("the argv[1] is %s\n",argv[1]);
    switch(*argv[1])
    {
        case '1':
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
            break;
        }
        case '2':
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
            break;
        }
        case '3':
        {
/****************linux_c 进程间通信实验********************/
            helloworld();

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

/************************c++实验******************************/
            char* c;
            void* p;
            p = &c;

            printf("p is %x\n", p);
            Compoent* m_compoent = new Compoent();
            if (m_compoent == NULL)
            {
                printf("get compoent error\n");
            }else{
                CSon& myson =  m_compoent->getson();
                CPerson& myperson = m_compoent->getperson();

                myson.SayTruth();
                myperson.SayTruth();

                myperson.set_my_age(50);
                myson.SayTruth();
                myperson.SayTruth();

                myson.set_age(21);
                myson.SayTruth();
                myperson.SayTruth();

                myson.sharenum = 10;
                myson.SayTruth();
                myperson.SayTruth();

                myson.ChangeWorld();
                myperson.ChangeWorld(10);

                myson.OutPutName("SON");
                myperson.OutPutName("FATHER");
                printf("get compoent successful\n");
            }

            if (argc >= 3)
            {
                my_fgets("/home/ronald/linux_c/config.ini");
                download_main(argv[2]);
            }

            //process_pthread();   //ptread
            forkRunProcess("forkandrun","/opt/mycode/bin/proces_one");

            msg_st* snddata;
            int msgid;
            char sndbuf[5] = "end";

            snddata = (msg_st*)malloc(sizeof(msg_st));
            memcpy(snddata->message,sndbuf,sizeof(sndbuf));
            snddata->msg_type = 1;

            if ((msgid = getMessage((key_t)1234,0666 | IPC_CREAT)) < 0)
            {
                printf("get message error\n");
            }

            while(1)
            {
                printf("this is main process and send data is %s\n",snddata->message);
                if (sendMessage(msgid,snddata,512,0))
                {
                    printf("main process send\n");
                }
                sleep(1);
            }
            free(snddata);
            break;
        }
        case '4':
        {
/********************文件编程实验**********************************/
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
            printf("the url ais %s\n" ,url.c_str());
            break;
        }
        case '5':
        {
            MyStl* mystl = MyStl::getMyStl();
            mystl->myqueue();
            mystl->myList();
            mystl->mySet();
            mystl->myVector();

            std::vector<int> *v =  new vector<int>;
            int a[5] = {1,2,2,3,3};
            int (*mychar)[5] = &a;
            mystl->getMyVector(v);
            printf("%d\n",(*v)[0] );
            printf("%d\n",(*mychar)[1] );
            for (typename vector<int>::iterator it = v->begin(); it != v->end(); it++)
            {
                int i = *it;
                cout <<  i;
            }
            cout << endl;
        break;
        }
        default:
        {
            printf("sholud not be here\n");
            break;
        }
    return 0;
}
}
