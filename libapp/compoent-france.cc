
#include "compoent-france.h"

CompoentFrance::CompoentFrance()
{
     printf("CompoentFrance called\n");
}

CompoentFrance::~CompoentFrance()
{
     printf("~CompoentFrance called\n");
}

bool CompoentFrance::start()
{
     printf("CompoentFrance start\n");
     return true;
}

bool CompoentFrance::stop()
{
     printf("CompoentFrance stop\n");
     return true;
}

int CompoentFrance::leave()
{
     printf("CompoentFrance leave\n");
}

int CompoentFrance::enter(string city, int times)
{
     printf("CompoentFrance enter\n");
}

int CompoentFrance::ChangeWorld()
{
     printf("CompoentFrance ChangeWorld\n");
}
