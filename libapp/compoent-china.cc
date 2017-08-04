
#include "compoent-china.h"

CompoentChina::CompoentChina()
{
    printf("CompoentChina called\n");
}

CompoentChina::~CompoentChina()
{
    printf("~CompoentChina called\n");
}

ComStatus CompoentChina::start()
{
    printf("CompoentChina start\n");
    return Comok;
}

ComStatus CompoentChina::stop()
{
    printf("CompoentChina stop\n");
    return Comok;
}

ComStatus CompoentChina::leave()
{
    printf("CompoentChina leave\n");
    return Comok;
}

ComStatus CompoentChina::enter(string city, int times)
{
    printf("it is %d times that CompoentChina enter;  the city is %s\n",times, city.c_str());
    return Comok;
}

int CompoentChina::ChangeWorld()
{
    printf("CompoentChina ChangeWorld\n");
}

int CompoentChina::creatWindow(int tableId)
{
    window newWindow =   creatNeWindow(tableId,true);
    table_window.insert(make_pair<int,window*>(tableId,&newWindow));

}

int CompoentChina::destroyWindow(int tableId)
{
    table_window.erase(tableId);
}