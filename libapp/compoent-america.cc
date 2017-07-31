
#include "compoent-america.h"

CompoentAmerica::CompoentAmerica()
{
     printf("CompoentAmerica called\n");
}

CompoentAmerica::~CompoentAmerica()
{
    printf("~CompoentAmerica called\n");
}

bool CompoentAmerica::start()
{
    printf("CompoentAmerica start\n");
    return true;
}

bool CompoentAmerica::stop()
{
    printf("CompoentAmerica stop\n");
    return true;
}

int CompoentAmerica::leave()
{
    printf("CompoentAmerica leave\n");
}

int CompoentAmerica::enter(string city, int times)
{
    printf("CompoentAmerica enter\n");
}

int CompoentAmerica::ChangeWorld()
{
    printf("CompoentAmerica ChangeWorld\n");
}