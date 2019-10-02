#ifndef TEST_H
#define TEST_H

#include "stdlib.h"
#include <stdio.h>
#include <malloc.h>
#include <iostream>

/*
 1.public和protected变量：base类和derived类共享
 2.即使是private继承，也符合第一条的原则
*/

class testParent
{
public:
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
protected:
    int aParent;

private:
	int bParent;
};

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

#endif