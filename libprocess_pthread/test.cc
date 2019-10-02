
#include "test.h"
#include "cmcc.h"
/*
************************关于继承************************
变量的继承可以理解为 共同使用    （通过子类修改变量，父类再查看变量，值是子类修改之后的）
*/

void testParent::testPrint(void)
{
    printf("testParent::testPrint\n");
}

void testParent::testNoVirtual(void)
{
    printf("testParent::testNoVirtual\n");
}

void testParent::test_protected_func()
{
    printf("testParent::test_protected_func\n");
}

void testParent::test_protected_func1()
{
    printf("testParent::test_protected_func1\n");
}

void testParent::test_protected_func2()
{
    printf("testParent::test_protected_func2\n");
}

/*
 1.public和protected变量：base类和derived类共享, 一块内存
 2.即使是private继承，也符合第一条的原则
*/
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

/*
 1.testPrint() 子类强制转换成父类之后，转换后的指针，也不能访问父类的虚函数，调用到的还是子类的函数，因为虚函数表是子类的。（特殊的隐藏）（重写）
 2.getB() 子类强制转换成父类之后，转换后的指针，继承过来的普通函数可以访问。
 3.testNoVirtual() 子类强制转换成父类之后，如果是父类和子类都有实现的一模一样的非虚函数。转换后的指针，可以访问父类的public函数
 4.子类的指针，c->父类::函数  访问父类的函数
 5.隐藏和protected pubilic 没有关系，只要父类和子类有一模一样的函数，子类都会隐藏父类函数。指针转换成父类后才可以访问父类的函数（public的）（一般的隐藏）
 6.virtual关键字和protected public 没有关系，只要父类和子类有一模一样的函数，都会形成多态，指针转换成父类，也只会访问到子类的函数。（特殊的隐藏）（重写）
*/
void test_virtual::testPrint(void)
{
    printf("test_virtual::testPrint\n");
}

void test_virtual::testNoVirtual(void)
{
    test_protected_func2();
    printf("test_virtual::testNoVirtual\n");
}

void test_virtual::test_protected_func()
{
    printf("test_virtual::test_protected_func\n");
}

void test_virtual::test_protected_func1()
{
    printf("test_virtual::test_protected_func1\n");
}

void test_virtual::test_protected_func2()
{
    printf("test_virtual::test_protected_func2\n");
}