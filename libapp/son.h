#ifndef SON_H
#define SON_H

#include <stdint.h>
#include "compoent.h"

#include "person.h"

//子类 类的继承 可以访问父类的public资源
class CSon:public CPerson
{
public:
    CSon(void);
    ~CSon(void);

    // override
    void play(void);
    void study(void);
    void work(void);
    void sleep(void);

    //Class Person static has no influnce on this
    void SayTruth(void);
    
    //example overwrite 
    void ChangeWorld(void);

    int set_age(int m_age);

    //void operator * (CSon&, CSon&); error: must take either zero or one argument
    void operator * (CSon&);
    void operator * (void);

private:
    int m_Son_age;
    int m_Son_salary;
    bool m_flag;
    int  his_age;
    void callback(int a, void(*p)(int));
    void SonSetAll(int m_age,int m_salay);
};

#endif