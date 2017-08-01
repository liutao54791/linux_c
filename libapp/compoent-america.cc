
#include "compoent-america.h"

CompoentAmerica::CompoentAmerica()
{
     printf("CompoentAmerica called\n");
}

CompoentAmerica::~CompoentAmerica()
{
    printf("~CompoentAmerica called\n");
}

ComStatus CompoentAmerica::start()
{
    printf("CompoentAmerica start\n");
    return Comok;
}

ComStatus CompoentAmerica::stop()
{
    printf("CompoentAmerica stop\n");
    return Comok;
}

ComStatus CompoentAmerica::leave()
{
    printf("CompoentAmerica leave\n");
    return Comok;
}

ComStatus CompoentAmerica::enter(string city, int times)
{
    printf("it is %d times that CompoentAmerica enter;  the city is %s\n",times, city.c_str());
    return Comok;
}

int CompoentAmerica::ChangeWorld()
{
    printf("CompoentAmerica ChangeWorld\n");
}