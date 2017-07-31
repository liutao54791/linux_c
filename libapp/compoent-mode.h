#ifndef COMPONET_MODE_H
#define COMPONET_MODE_H

#include <stdint.h>
#include "compoent.h"
#include <string>
#include "person.h"


enum ComMode
{
    Modeoff,
    China,
    America,
    England,
    France
};
//子类 类的继承 可以访问父类的public资源
class CompoentMode
{
public:
	
    virtual bool start() = 0;
    virtual bool stop() = 0;
    virtual int leave() = 0;
    virtual int enter(std::string city, int times) = 0;
    virtual int ChangeWorld() = 0;

private:

};

#endif