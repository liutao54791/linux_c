#ifndef COMPOENT_H
#define COMPOENT_H

#include <stdint.h>
#include <map>
#include <memory>
#include <string>
#include <stdio.h>

#include "person.h"
#include "son.h"
#include "grandson.h"
#include "compoent-mode.h"

using namespace std;
class CompoentMode;
class GrandSon;
class CPerson;
class CSon;

class Compoent
{
public:
    //static Compoent* getInstance();
    Compoent(void);
    ~Compoent();
    void CreatCompoent(void);
    int addCpersonToMap(CPerson* person);
    bool startcompoents();
    bool stopcompoents();
    CPerson& getperson(void);
    CSon& getson(void);
    GrandSon& getgrandson(void);

private:
    ComMode m_currentmode ;
    CPerson* m_person = NULL;
    CSon* m_son = NULL;
    GrandSon* m_grandson = NULL;

    std::map<ComMode, CompoentMode*> MapMode;

};

#endif
