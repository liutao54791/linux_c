
#include "compoent-france.h"

CompoentFrance::CompoentFrance()
{
     printf("CompoentFrance called\n");
}

CompoentFrance::~CompoentFrance()
{
     printf("~CompoentFrance called\n");
}

ComStatus CompoentFrance::start()
{
     printf("CompoentFrance start\n");
     return Comok;
}

ComStatus CompoentFrance::stop()
{
     printf("CompoentFrance stop\n");
     return Comok;
}

ComStatus CompoentFrance::leave()
{
     printf("CompoentFrance leave\n");
     return Comok;
}

ComStatus CompoentFrance::enter(string city, int times)
{
     printf("it is %d times that CompoentFrance enter;  the city is %s\n",times, city.c_str());
     return Comok;
}

int CompoentFrance::ChangeWorld()
{
     printf("CompoentFrance ChangeWorld\n");
}
