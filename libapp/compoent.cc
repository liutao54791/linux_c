
#include "compoent.h"

#include "compoent-england.h"
#include "compoent-china.h"
#include "compoent-america.h"
#include "compoent-france.h"

Compoent::Compoent(void)
:m_currentmode(Modeoff)
{
    printf("Compoent called\n");
}

Compoent::~Compoent(void)
{
    printf("~Compoent called\n");
}

/*Compoent* Compoent::getInstance()
{
    static Compoent* m_compoent = NULL;
    if (m_compoent == NULL)
    {
        m_compoent = new Compoent();
        m_compoent->CreatCompoent();
    }
    return m_compoent;
}*/

void Compoent::CreatCompoent(void)
{
    printf("Compoent::CreatCompoent called\n");
    if (m_person == NULL)
    {
        m_person = new CPerson();
    }

    if (m_son == NULL)
    {
        m_son = new CSon();
    }

    if (m_grandson == NULL)
    {
        m_grandson = new GrandSon();
    }

    MapMode[ China ]     = (CompoentMode*)new CompoentChina();
    MapMode[ America ] = (CompoentMode*)new CompoentAmerica();
    MapMode[ England ] = (CompoentMode*)new CompoentEngland();
    MapMode[ France ]   = (CompoentMode*)new CompoentFrance();
}

bool Compoent::startcompoents()
{
    printf("Compoent::startcompoents called\n");
    bool status = false;
    status = MapMode[ China ]->start();
    if (!status)
    {
        printf("China start failed\n");
        return false;
    }
    
    status = MapMode[ America ]->start();
    if (!status)
    {
        printf("America start failed\n");
        return false;
    }
    
    status = MapMode[ England ]->start();
    if (!status)
    {
        printf("England start failed\n");
        return false;
    }
    
    status = MapMode[ France ]->start();
    if (!status)
    {
        printf("France start failed\n");
        return false;
    }
    return true;
}

bool Compoent::stopcompoents()
{
    printf("Compoent::stopcompoents called\n");
    bool status = false;
    status = MapMode[ China ]->stop();
    if (!status)
    {
        printf("China stop failed\n");
        return false;
    }
    
    status = MapMode[ America ]->stop();
    if (!status)
    {
        printf("America stop failed\n");
        return false;
    }
    
    status = MapMode[ England ]->stop();
    if (!status)
    {
        printf("England stop failed\n");
        return false;
    }
    
    status = MapMode[ France ]->stop();
    if (!status)
    {
        printf("France stop failed\n");
        return false;
    }
    return true;
}

ComMode Compoent::enter(ComMode mode,string city, int times)
{
    printf("Compoent::enter\n");
    if ((mode >= Modeoff) && (mode <= France))
    {
        MapMode[ m_currentmode ]->leave();
        m_currentmode = mode;
        MapMode[ m_currentmode ]->enter(city,times);
        return m_currentmode;
    }
    printf("Compoent::enter error\n");
}

ComMode Compoent::leave(ComMode mode)
{
    printf("Compoent::leave\n");
    if ((mode >= Modeoff) && (mode <= France))
    {
        m_currentmode = mode;
        MapMode[ m_currentmode ]->leave();
        return m_currentmode;
    }
    printf("Compoent::leave error\n");
}

CPerson& Compoent::getperson(void)
{
    if (m_person != NULL)
    {
        return *m_person;
    }
    printf("m_person is null\n");
}

CSon& Compoent::getson(void)
{
    if (m_son != NULL)
    {
        return *m_son;
    }
    printf("m_son is null\n");
}

GrandSon& Compoent::getgrandson(void)
{
    if (m_grandson != NULL)
    {
        return *m_grandson;
    }
    printf("m_grandson is null\n");
}
