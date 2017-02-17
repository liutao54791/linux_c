#ifndef GRANDSON_H
#define GRANDSON_H

#include <stdint.h>
#include "person.h"

//子类 类的继承 可以访问父类的public资源
class GrandSon:public CPerson
{
public:
    void SayTruth(void);
};

#endif
