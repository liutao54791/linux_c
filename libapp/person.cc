

#include <stdio.h>
#include <string.h>
#include "person.h"
#include "cmcc.h"

using namespace std;
//using namespace cmcc;

int CPerson::sharenum = 100;

CPerson::CPerson(void)
{
    my_true_age = 26;
    my_true_salary = 8000;
    age = 40;
    fprintf(stdout,"this is class CPerson\r\n");
}

CPerson::~CPerson(void)
{
    fprintf(stdout,"good bye class CPserson\r\n");
}

int CPerson::set_my_age(int m_age)
{
    age = m_age;
    fprintf(stdout,"father set age \r\n");
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

void CPerson::OutPutName(std::string mystring)
{
    std::string myname = mystring;
    printf("Person Name is %s\r\n",myname.c_str());

}

void CPerson::SayTruth(void)
{
    fprintf(stdout,"father age is %d\r\n",age);
    fprintf(stdout,"Person sharenum is %d\r\n",sharenum);
    printf("we should be a good person\n");
}

void CPerson::play(void)
{
    printf("person need to play\n");
}

void CPerson::work(void)
{
    printf("person need to work\n");
}

void CPerson::study(void)
{
    printf("person nedd to study\n");
}

void CPerson::sleep(void)
{
    printf("person need to sleep\n");
}

void CPerson::SingSongs(void)
{
    //printf("person sing song\n");
    cmcc_info::getInstance()->cmcc_string = 6;
    printf("cmcc::cmcc_info::getInstance()->cmcc_string:%d\n",cmcc_info::getInstance()->cmcc_string);
}

void CPerson::ChangeWorld(int changenum)
{
    printf("Person changenum is %d\n",changenum);
}
