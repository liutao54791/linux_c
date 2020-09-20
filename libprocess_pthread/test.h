#ifndef TEST_H
#define TEST_H

#include "stdlib.h"
#include <stdio.h>
#include <malloc.h>
#include <iostream>

class grandParent
{
public:
	inline void release()
	{
		printf("this is grandParent release\n");
		clear();
	};

	virtual void clear() = 0;

};

class testParent:public grandParent
{
public:
	testParent()
	{
	};

	testParent(int a, int b)
	{
		printf("this is testParent construct\n");
		aParent = a;
		bParent = b;
	};
	inline int getA()
	{
		printf("this is testParent getA %d\n", aParent);
		return aParent;
	}
	inline int getB()
	{
		printf("this is testParent getB %d\n", bParent);
		return bParent;
	}
	~testParent()
	{
		printf("this is testParent de construct\n");
	};

	virtual void testPrint(void);
	void testNoVirtual(void);
	void test_protected_func2();

protected:
    int aParent;
	void test_protected_func();
	virtual void test_protected_func1();

private:
	int bParent;
	virtual void testPrivateVirtual();
};


/*
 1.testPrint() 子类强制转换成父类之后，转换后的指针，也不能访问父类的虚函数，调用到的还是子类的函数，因为虚函数表是子类的。（特殊的隐藏）（重写）
 2.getB() 子类强制转换成父类之后，转换后的指针，继承过来的普通函数可以访问。
 3.testNoVirtual() 子类强制转换成父类之后，如果是父类和子类都有实现的一模一样的函数。转换后的指针，可以访问父类的函数
 4.子类的指针，c->父类::函数  访问父类的函数
 5.隐藏和protected pubilic 没有关系，只要父类和子类有一模一样的函数，子类都会隐藏父类函数。指针转换成父类后才可以访问父类的函数（public的）（一般的隐藏）
 6.virtual关键字和protected public 没有关系，只要父类和子类有一模一样的函数，都会形成多态，指针转换成父类，也只会访问到子类的函数。（特殊的隐藏）（重写）
*/
class test_virtual:public testParent
{
public:
    test_virtual(){};
	void testPrint(void);
	void testNoVirtual(void);
	void test_protected_func();
	void test_protected_func1();
	void clear();
	virtual void testPrivateVirtual();
    ~test_virtual(){};

protected:
    void test_protected_func2();
};

/*
 1.public和protected变量：base类和derived类共享
 2.即使是private继承，也符合第一条的原则
*/
class test:private testParent
{
public:
    test();
    void setA(int a);
    int getA();
    void setB(int b);
    int getB();
    ~test();
};

/*
************************关于继承************************
变量的继承可以理解为 共同使用
*/

#endif