
#ifndef _LIBSO_H
#define _LIBSO_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <map>

/**@brief Application main function.
 */
int helloworld(void);
int my_fgets(char* filename);

struct window
{
    int tableid;
    bool  transparent;
};

window creatNewWindow(int tableId,bool trans);

#endif
