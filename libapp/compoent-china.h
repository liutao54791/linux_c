#ifndef COMPONET_CHINA_H
#define COMPONET_CHINA_H

#include <stdint.h>
#include "libso.h"
#include "person.h"
#include "compoent.h"
#include "compoent-mode.h"

//子类 类的继承 可以访问父类的public资源
class CompoentChina:public CompoentMode
{
public:
    CompoentChina(void);
    ~CompoentChina(void);

    virtual ComStatus start();
    virtual ComStatus stop();
    virtual ComStatus leave();
    virtual ComStatus enter(string city, int times);
    virtual int ChangeWorld();
    int CreatWalls();
    int creatWindow(int tableId);
    int destroyWindow(int tableId);

private:
    std::map<int, window*> table_window;
};

#endif