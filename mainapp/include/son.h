#ifndef SON_H
#define SON_H

#include <stdint.h>
#include "person.h"

//子类 类的继承 可以访问父类的public资源
class CSon:public CPerson
{
public:
    CSon(void);
    ~CSon(void);

    void play(void);
    void study(void);
    void work(void);
    void sleep(void);
    void SayTruth(void);

private:
    int m_Son_age;
    int m_Son_salary;
    bool m_flag;
    int  his_age;
    void callback(int a, void(*p)(int));
    void SonSetAll(int m_age,int m_salay);
};

#endif