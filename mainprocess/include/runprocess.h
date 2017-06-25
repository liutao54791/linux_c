#ifndef RUNPROCESS_H
#define RUNPROCESS_H

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
#include <sys/ipc.h>

#include <sys/msg.h>

using namespace std;


std::string forkRunProcess(std::string command,char* processName);

#endif