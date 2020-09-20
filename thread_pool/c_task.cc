#include <string>
#include <vector>
#include <iostream>

#include "c_task.h"


c_task::c_task()
{

}

c_task::~c_task()
{

}

void c_task::process_task()
{
	printf("this is c_task process_task!\n");
	sleep(2);
}