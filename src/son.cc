#include <iostream>
#include <stdio.h>
#include <string.h>

#include "son.h"

CSon::CSon(void)
{
    m_flag = true;
}

CSon::~CSon(void)
{
    m_flag = false;
}

void CSon::SonSetAll(int m_age,int m_salary)
{
    age = m_age;
    salary = m_salary;
    printf("this is class son\r\n");
    printf("son age and salary is %d %d\r\n",age,salary);
    
}

void CSon::OutPutName(void)
{
    printf("hello CSon \r\n");
}

void CSon::callback(int a, void(*p)(int))
{
    char i = 0;
    int  count;
    count = a;
    for (i = 0; i < count; i++)
    {
        p(i);
    }
}

void CSon::SayTruth(void)
{
    printf("son is a good person\n");
}