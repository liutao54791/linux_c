#ifndef COMPONET_CHINA_H
#define COMPONET_CHINA_H

#include <stdint.h>
#include "person.h"
#include "compoent.h"
#include "compoent-mode.h"

//子类 类的继承 可以访问父类的public资源
class CompoentChina:public CompoentMode
{
public:
    CompoentChina(void);
    ~CompoentChina(void);

    virtual bool start();
    virtual bool stop();
    virtual int leave();
    virtual int enter(string city, int times);
    virtual int ChangeWorld();
    int CreatWalls();
private:

};

#endif