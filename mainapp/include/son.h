#ifndef SON_H
#define SON_H

#include <stdint.h>
#include "person.h"

//子类 类的继承 可以访问父类的public资源
class CSon:public CPerson
{
public:
    int m_Son_age;
    int m_Son_salary;
    
    CSon(void);
    ~CSon(void);

    void callback(int a, void(*p)(int));
    void SonSetAll(int m_age,int m_salay);
    void OutPutName(void);
    void SayTruth(void);

private:
    bool m_flag;
    int  his_age;
};

#endif