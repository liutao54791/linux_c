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

#include "compoent-mode.h"
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
#include "test.h"

#include "sort_algorithm.h"

int main(int argc, char* argv[])
{
  test* b = new test();
  b->setA(12);

  printf("this is %d\n",b->getA());
  printf("this is %d\n",b->getB());

  sort_main();
}
