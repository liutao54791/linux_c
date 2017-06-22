
#include "myfifo.h"


bool close_fifo(int fd)
{
    close(fd);
}

//进程间通信-有名管道
bool creat_fifo(mode_t  mode,char* fifo_name)
{
    if (mkfifo(fifo_name,mode) < 0)
    {
        perror("make fifo error");
        return false;
    }else {
        printf("the fifo name is %s\n", fifo_name);
        return true;
    }
}

bool read_fifo(char* readbuf, char* fifo_name,int length)
{
    int    fd;
    int    len;

    if((fd = open(fifo_name, O_RDONLY)) < 0)
    {
        perror("open fifo failed");
        return false;
    }

    while((len = read(fd, readbuf,length)) > 0)
    {
        printf("read fifo is %s\n", readbuf);
    }
    close_fifo(fd);
    return true;

}

bool write_fifo(char* write_string , char* fifo_name)
{
    int     fd;
    int     n;

    int WRITELENGH = strlen(write_string) +1;
    char    buf[WRITELENGH];

    if ((fd = open(fifo_name, O_WRONLY)) < 0)
    {
        perror("open error");
        return false;
    }

    n = sprintf(buf,"write_text %s",write_string);

    printf("Send msg:%s",buf);

    if((write(fd, buf, n+1)) < 0)
    {    /*写入到FIFO中*/
        perror("write error");
    }
    close_fifo(fd);  /*关闭FIFO文件*/
    return true;
/*
    for ( i=0 ; i<10; i++)
    {
        time(&tp); 

        n = sprintf(buf,"write_fifo %d sends %s",getpid(),ctime(&tp));

        printf("Send msg:%s",buf);

        if((write(fd, buf, n+1)) < 0)
        {
            perror("write");
            close(fd);
        }
    }
   */
}
