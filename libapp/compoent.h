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

class Compoent
{
public:
	static Compoent* getInstance();
	void CreatCompoent(void);
	CPerson& getperson(void);
    CSon& getson(void);
    GrandSon& getgrandson(void);
private:
    CPerson* m_person = NULL;
    CSon* m_son = NULL;
    GrandSon* m_grandson = NULL;

};

#endif
