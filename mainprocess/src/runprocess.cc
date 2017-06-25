
#include "runprocess.h"

std::string forkRunProcess(std::string command,char* processName)
{
    if (command == "forkandrun")
    {
        pid_t pid;
        if ((pid = fork()) == 0)
        {
            if (execl(processName, NULL) < 0)
            {
                perror("exec errors");
                return "error";
            }
        }
        return "success";
    }
    return "nothing";
}