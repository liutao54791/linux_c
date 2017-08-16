
#include "libso.h"


/**@brief Application main function.
 */

int helloworld(void)
{
    printf("hello libso\n");
}

int my_fgets(char* filename)
{
    FILE* fp;
    char buf[1024];
    if (filename != NULL)
    {
        if ((fp = fopen(filename,"r")) == NULL)
        {
            perror("open file errors");
            return -1;
        }
        while(!feof(fp))                        //read all  lines
        {
            fgets(buf,sizeof(buf),fp);    //  read one ine
            printf("buf is %s", buf);
        }
        fclose(fp);
    }
    return 0;
}

window creatNewWindow(int tableId,bool trans)
{
    static  int windowNum = 0;
    window m_window;
    m_window.tableid = tableId;
    m_window.transparent = trans;
    windowNum ++;

    return m_window;
}

