#ifndef COMPONET_ENGLAND_H
#define COMPONET_ENGLAND_H

#include <stdint.h>
#include "person.h"
#include "compoent.h"
#include "compoent-mode.h"

//子类 类的继承 可以访问父类的public资源
class CompoentEngland:public CompoentMode
{
public:
    CompoentEngland(void);
    ~CompoentEngland(void);

    virtual ComStatus start();
    virtual ComStatus stop();
    virtual ComStatus leave();
    virtual ComStatus enter(string city, int times);
    virtual int ChangeWorld();

private:

};

#endif