

#include <stdio.h>
#include <string.h>
#include "person.h"

CPerson::CPerson(void)
{
    my_true_age = 26;
    my_true_salary = 8000;
    
    fprintf(stdout,"this is class CPerson\r\n");
}
CPerson::~CPerson(void)
{
    fprintf(stdout,"good bye class CPserson\r\n");
}
int CPerson::set_my_age(int m_age)
{
    age = m_age;
    fprintf(stdout,"father age is %d\r\n",age);
    
    return age;
}

int CPerson::set_my_salary(int m_salary)
{
    salary = m_salary;
    fprintf(stdout,"father salary is %d\r\n",salary);
    
    return salary;
}

void CPerson::SetNodeName(const char *pchData)
{
    if(pchData != NULL)
    {
        strcpy(m_Node.m_Name,pchData);
    }
    printf("m_Node.m_Name is %s\r\n",m_Node.m_Name);

}

void CPerson::OutPutName(void)
{
    printf("hello CPerson\r\n");

}

void CPerson::SayTruth(void)
{
    printf("father is a good person\n");
}