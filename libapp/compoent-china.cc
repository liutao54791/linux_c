
#include "compoent-china.h"

CompoentChina::CompoentChina()
{
    printf("CompoentChina called\n");
}

CompoentChina::~CompoentChina()
{
    printf("~CompoentChina called\n");
}

bool CompoentChina::start()
{
    printf("CompoentChina start\n");
    return true;
}

bool CompoentChina::stop()
{
    printf("CompoentChina stop\n");
    return true;
}

int CompoentChina::leave()
{
    printf("CompoentChina leave\n");
}

int CompoentChina::enter(string city, int times)
{
    printf("CompoentChina enter\n");
}

int CompoentChina::ChangeWorld()
{
    printf("CompoentChina ChangeWorld\n");
}