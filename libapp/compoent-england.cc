
#include "compoent-england.h"

CompoentEngland::CompoentEngland()
{
     printf("CompoentEngland called\n");
}

CompoentEngland::~CompoentEngland()
{
     printf("~CompoentEngland called\n");
}

bool CompoentEngland::start()
{
     printf("CompoentEngland start\n");
     return true;
}

bool CompoentEngland::stop()
{
     printf("CompoentEngland stop\n");
     return true;
}

int CompoentEngland::leave()
{
     printf("CompoentEngland leave\n");
}

int CompoentEngland::enter(string city, int times)
{
    printf("it is %d times that CompoentEngland enter;  the city is %s\n",times, city.c_str());
}

int CompoentEngland::ChangeWorld()
{
     printf("CompoentEngland ChangeWorld\n");
}