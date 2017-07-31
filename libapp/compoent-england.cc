
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
     printf("CompoentEngland enter\n");
}

int CompoentEngland::ChangeWorld()
{
     printf("CompoentEngland ChangeWorld\n");
}