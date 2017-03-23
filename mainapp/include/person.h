

#ifndef PERSON_H
#define PERSON_H

//#include <iostream>
#include <stdint.h>

class CPerson
{
public:
    
    //类嵌套
    class CNode
    {
    private:
        int m_tag;
    public:
        char m_Name[20];
    };
    
    int age;
    int salary;
    CNode m_Node;
    
    CPerson(void);   //构造函数
    ~CPerson(void);  //析构函数
    virtual void play(void);
    virtual void study(void);
    virtual void work(void);
    virtual void sleep(void);
    virtual void SayTruth(void);

    int set_my_age(int m_age);
    int set_my_salary(int m_salary);
    void SetNodeName(const char *pchData);
    void OutPutName(void);

private:
    
    int my_true_age;
    int my_true_salary;
};

#endif
