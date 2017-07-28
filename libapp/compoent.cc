
#include "compoent.h"

Compoent::Compoent(void)
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

    Mapperson[ Son ] = (CPerson*)new CSon();
    Mapperson[ Grandson ] = (CPerson*)new GrandSon();
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