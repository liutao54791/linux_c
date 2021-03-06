#ifndef COMPONET_MODE_H
#define COMPONET_MODE_H

#include <stdint.h>
#include <string>


enum ComMode
{
    Modeoff = 0,
    China,
    America,
    England,
    France
};

enum ComStatus
{
    Comok = 0,
    Comfailed
};
//子类 类的继承 可以访问父类的public资源
class CompoentMode
{
public:
	
    virtual ComStatus start() = 0;
    virtual ComStatus stop() = 0;
    virtual ComStatus leave() = 0;
    virtual ComStatus enter(std::string city, int times) = 0;
    virtual int ChangeWorld() = 0;

private:

};

#endif