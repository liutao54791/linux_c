#ifndef GRANDSON_H
#define GRANDSON_H

#include <stdint.h>
#include "person.h"

//子类 类的继承 可以访问父类的public资源
class GrandSon:public CPerson
{
public:
	GrandSon(void);
    ~GrandSon(void);
    
    //override
    void play(void);
    void study(void);
    void work(void);
    void sleep(void);
    void SayTruth(void);
};

#endif
