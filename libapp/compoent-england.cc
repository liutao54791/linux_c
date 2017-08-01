
#include "compoent-england.h"

CompoentEngland::CompoentEngland()
{
     printf("CompoentEngland called\n");
}

CompoentEngland::~CompoentEngland()
{
     printf("~CompoentEngland called\n");
}

ComStatus CompoentEngland::start()
{
     printf("CompoentEngland start\n");
     return Comok;
}

ComStatus CompoentEngland::stop()
{
     printf("CompoentEngland stop\n");
     return Comok;
}

ComStatus CompoentEngland::leave()
{
     printf("CompoentEngland leave\n");
     return Comok;
}

ComStatus CompoentEngland::enter(string city, int times)
{
    printf("it is %d times that CompoentEngland enter;  the city is %s\n",times, city.c_str());
    return Comok;
}

int CompoentEngland::ChangeWorld()
{
     printf("CompoentEngland ChangeWorld\n");
}