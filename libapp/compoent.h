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

using namespace std;
class GrandSon;
class CPerson;
class CSon;

enum Mode
{
    Son,
    Grandson
};

enum ComMode
{
    China,
    America,
    England,
    France
};

class Compoent
{
public:
    //static Compoent* getInstance();
    Compoent(void);
    ~Compoent();
    void CreatCompoent(void);
    CPerson& getperson(void);
    CSon& getson(void);
    GrandSon& getgrandson(void);
    int addCpersonToMap(CPerson* person);
private:
    CPerson* m_person = NULL;
    CSon* m_son = NULL;
    GrandSon* m_grandson = NULL;

    std::map<Mode, CPerson*> Mapperson;

};

#endif
