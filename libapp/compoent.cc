
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
    status = MapMode.at(China)->start();
    if (!status)
    {
        printf("China start failed\n");
        return false;
    }
    
    status = MapMode.at(America)->start();
    if (!status)
    {
        printf("America start failed\n");
        return false;
    }
    
    status = MapMode.at(England)->start();
    if (!status)
    {
        printf("England start failed\n");
        return false;
    }
    
    status = MapMode.at(France)->start();
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
    status = MapMode.at(China)->stop();
    if (!status)
    {
        printf("China stop failed\n");
        return false;
    }
    
    status = MapMode.at(America)->stop();
    if (!status)
    {
        printf("America stop failed\n");
        return false;
    }
    
    status = MapMode.at(England)->stop();
    if (!status)
    {
        printf("England stop failed\n");
        return false;
    }
    
    status = MapMode.at(France)->stop();
    if (!status)
    {
        printf("France stop failed\n");
        return false;
    }
    return true;
}

ComMode Compoent::entermode(ComMode mode,string city, int times)
{
    printf("Compoent::enter\n");
    if ((mode >= Modeoff) && (mode <= France))
    {
        if (m_currentmode != Modeoff)
        {
            MapMode.at(m_currentmode)->leave();
        }
        m_currentmode = mode;
        MapMode.at(m_currentmode)->enter(city,times);
        return m_currentmode;
    }
    printf("Compoent::enter error\n");
}

ComMode Compoent::leavemode(ComMode mode)
{
    printf("Compoent::leave\n");

    if (m_currentmode != Modeoff)
    {
        MapMode.at(m_currentmode)->leave();
    }
    return m_currentmode;
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
