
#include "test.h"
#include "cmcc.h"


test::test():testParent(55,66)
{}

void test::setA(int a)
{
	aParent = a;
    printf("this is test setA %d\n",aParent);
}

int test::getA()
{
    printf("getA from parent %d\n",testParent::getA());
    printf("this is test getA %d\n",aParent);
    return  aParent;
}

int test::getB()
{
    printf("this is test getB %d\n",testParent::getB());
    return testParent::getB();
}

test::~test()
{
}