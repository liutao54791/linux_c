#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <sys/wait.h>

#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#include "compoent-mode.h"
#include "compoent.h"
#include "person.h"
#include "son.h"
#include "grandson.h"
#include "libso.h"
#include "uart.h"
#include "m_pthread.h"
#include "socket_http_download.h"
#include "message.h"
#include "runprocess.h"
#include "mystl.h"
#include "test.h"
#include "sort_algorithm.h"
#include "c_thread.h"
#include "c_thread_manager.h"

/*
 1.testPrint() 子类强制转换成父类之后，转换后的指针，也不能访问父类的虚函数，调用到的还是子类的函数，因为虚函数表是子类的。（特殊的隐藏）（重写）
 2.getB() 子类强制转换成父类之后，转换后的指针，继承过来的普通函数可以访问。
 3.testNoVirtual() 子类强制转换成父类之后，如果是父类和子类都有实现的一模一样的函数。转换后的指针，可以访问父类的函数
 4.子类的指针，c->父类::函数  访问父类的函数
 5.隐藏和protected pubilic 没有关系，只要父类和子类有一模一样的函数，子类都会隐藏父类函数。指针转换成父类后才可以访问父类的函数（public的）（一般的隐藏）
 6.virtual关键字和protected public 没有关系，只要父类和子类有一模一样的函数，都会形成多态，指针转换成父类，也只会访问到子类的函数。（特殊的隐藏）（重写）
*/
//void test_virtual_main()
//{
//    test_virtual* c = new test_virtual();
//    c->testPrint();
//    c->testNoVirtual();          //父类的是public，子类的是protected，依然会隐藏 test_protected_func2
//    c->test_protected_func();    //父类的是protected，子类的是public，依然会隐藏
//    c->test_protected_func1();
//    c->release();
//    c->testParent::testPrint();
//    c->testParent::testNoVirtual();
//
//    printf("\n\n");
//
//    testParent* cToa = dynamic_cast<testParent*>(c);
//    cToa->testPrint();            //调用子类的函数，虚函数表指针指向的是子类的虚函数表 所以可以理解为 重写 了该函数
//    cToa->testNoVirtual();        //调用父类的函数
//    //cToa->test_protected_func();  作用域外不能调用
//    //cToa->test_protected_func1(); 作用域外不能调用
//    cToa->test_protected_func2();  //调用父类的函数
//
//    testParent* cToa_1 = c;
//    //cToa_1->testPrivateVirtual();  //不允许调用，因为父类的是private，即使子类的是public
//    //cToa_1->test_protected_func1(); 转换之后不能访问， 作用域外不能调用
//
//}

int  main()
{
    int i = 2;
    switch(i)
    {
        case 1:i++;
        case 2:i--;
        case 3:++i;break;
        case 4:--i;
        default: i++;
    }
    printf("i：%d",i);
    return 0;
}