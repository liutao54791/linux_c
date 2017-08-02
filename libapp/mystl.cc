
#include "mystl.h"

/*
string s1;//初始化字符串，空字符串
string s2 = s1; //拷贝初始化，深拷贝字符串
string s3 = "I am Yasuo"; //直接初始化，s3存了字符串
string s4(10, 'a'); //s4存的字符串是aaaaaaaaaa
string s5(s4); //拷贝初始化，深拷贝字符串
string s6("I am Ali"); //直接初始化
string s7 = string(6, 'c'); //拷贝初始化，cccccc
*/
MyStl* MyStl::getMyStl(void)
{
    static MyStl* mystl = NULL;
    if (mystl = NULL)
    {
        mystl = new MyStl();
        return mystl;
    }
}

int MyStl::string_base(void)
{
    string s1;//初始化字符串，空字符串
    string s2 = s1; //拷贝初始化，深拷贝字符串
    string s3 = "I am Yasuo"; //直接初始化，s3存了字符串
    string s4(10, 'a'); //s4存的字符串是aaaaaaaaaa
    string s5(s4); //拷贝初始化，深拷贝字符串
    string s6("I am Ali"); //直接初始化
    string s7 = string(6, 'c'); //拷贝初始化，cccccc

    //string的各种操作
    string s8 = s3 + s6;//将两个字符串合并成一个
    s3 = s6;//用一个字符串来替代另一个字符串的对用元素

    cin >> s1;

    cout << s1 << endl;
    cout << s2 << endl;
    cout << s3 << endl;
    cout << s4 << endl;
    cout << s5 << endl;
    cout << s6 << endl;
    cout << s7 << endl;
    cout << s8 << endl;
    cout << "s7 size = " << s7.size() << endl; //字符串长度，不包括结束符
    cout << (s2.empty() ? "This string is empty" : "This string is not empty") << endl;;

    string sq("heoolo sdaa ss");
    cout << sq.find("aa", 0) << endl; //返回的是子串位置。第二个参数是查找的起始位置，如果找不到，就返回string::npos
    if (sq.find("aa1", 0) == string::npos)
    {
        cout << "找不到该子串！" << endl;
    }


    return 0;
}

/*
C++ STL中的verctor好比是C语言中的数组，但是vector又具有数组没有的一些高级功能。与数组相比，
vector就是一个可以不用再初始化就必须制定大小的边长数组，当然了，它还有许多高级功能。

vector<int> v1;
vector<father> v2;
vector<string> v3;
vector<vector<int> >;  //注意空格。这里相当于二维数组int a[n][n];
vector<int> v5 = { 1,2,3,4,5 }; //列表初始化,注意使用的是花括号
vector<string> v6 = { "hi","my","name","is","lee" };
vector<int> v7(5, -1); //初始化为-1,-1,-1,-1,-1。第一个参数是数目，第二个参数是要初始化的值
vector<string> v8(3, "hi");
vector<int> v9(10); //默认初始化为0
*/
/*
如何向vector添加元素？

请使用push_back加入元素，并且这个元素是被加在数组尾部的。

for (int i = 0; i < 20; i++)
{
    v1.push_back(i);
}

注意：只能对已存在的元素进行赋值或者修改操作，如果是要加入新元素，务必使用push_back。
push_back的作用有两个：告诉编译器为新元素开辟空间、将新元素存入新空间里。
比如下面的代码是错误的，但是编译器不会报错，就像是数组越界。

vector<int> vec;
vec[0] = 1;  //错误！

当然我们也可以选择使用迭代器来访问元素

vector<string> v6 = { "hi","my","name","is","lee" };
for (vector<string>::iterator iter = v6.begin(); iter != v6.end(); iter++)
{
    cout << *iter << endl;
    //下面两种方法都行
    cout << (*iter).empty() << endl;
    cout << iter->empty() << endl; 
}

上面是正向迭代，如果我们想从后往前迭代该如何操作？
使用反向迭代器

for (vector<string>::reverse_iterator iter = v6.rbegin(); iter != v6.rend(); iter++)
{
    cout << *iter << endl;

}
*/

template <typename T>
void MyStl::showVector(vector<T> v)
{
    for (typename vector<T>::iterator it = v.begin(); it != v.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
}

template <typename T>
void MyStl::showList(list<T> v)
{
    for (typename list<T>::iterator it = v.begin(); it != v.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
}

template <typename T>
void MyStl::showSet(set<T> v)
{
    for (typename set<T>::iterator it = v.begin(); it != v.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
}

int MyStl::myVector(void)
{
    string mystring[] = {"hi","my","name","is","lee"};
    size_t  mystringCount=sizeof(mystring)/sizeof(string);
    vector<string> v6 (mystring,mystring + mystringCount);
    v6.resize(3);  //重新调整vector容量大小
    showVector(v6);

    int myInt[] = {1,2,3,4,5 };
    size_t  myIntCount=sizeof(myInt)/sizeof(int);

    vector<int> v5 (myInt, myInt + myIntCount); //列表初始化,注意使用的是花括号
    cout << v5.front() << endl; //访问第一个元素
    cout << v5.back() << endl; //访问最后一个元素

    showVector(v5);
    v5.pop_back(); //删除最后一个元素
    showVector(v5);
    v5.push_back(6); //加入一个元素并把它放在最后
    showVector(v5);
    v5.insert(v5.begin()+1,9); //在第二个位置插入新元素
    showVector(v5);
    v5.erase(v5.begin() + 3);  //删除第四个元素
    showVector(v5);
    v5.insert(v5.begin() + 1, 7,8); //连续插入7个8
    showVector(v5);
    v5.clear(); //清除所有内容
    showVector(v5);

    return 0;
}

int MyStl::getMyVector( std::vector<int> *v)
{
    int Int[] = {1,2,3,4,5 };
    size_t IntCount=sizeof(Int)/sizeof(int);
    std::vector<int> vx (Int, Int + IntCount); //列表初始化,注意使用的是花括号
    for (typename vector<int>::iterator it = vx.begin(); it != vx.end(); it++)
    {
        int i = *it;
        cout <<  i;
        v->push_back(i);
    }
    cout << endl;

}
/*
set跟vector差不多，它跟vector的唯一区别就是，set里面的元素是有序的且唯一的，
只要你往set里添加元素，它就会自动排序，而且，如果你添加的元素set里面本来就存在，
那么这次添加操作就不执行。要想用set先加个头文件set。
*/

int MyStl::mySet(void)
{
    int a[] = {9,8,1,2,3,4,5,5,5,6,7,7 };
    size_t  count=sizeof(a)/sizeof(int);
    set<int> s1(a, a+count); //自动排序，从小到大,剔除相同项
    showSet(s1);
    string mystring[] = {"hello","sysy","school","hello"};
    size_t  mystringCount=sizeof(mystring)/sizeof(string);
    set<string> s2 (mystring,mystring+mystringCount );      //字典序排序
    showSet(s2);
    s1.insert(9); //有这个值了，do nothing
    showSet(s1);
    s2.insert("aaa"); //没有这个字符串，添加并且排序
    showSet(s2);

    return 0;
}

/*
list就是链表，在C语言中我们想使用链表都是自己去实现的，实现起来倒不难，
但是如果有现成的高效的链表可以使用的话，我们就不需要重复造轮子了。STL就提供了list容器给我们。
list是一个双向链表，而单链表对应的容器则是foward_list。
list即双向链表的优点是插入和删除元素都比较快捷，缺点是不能随机访问元素。
初始化方式就大同小异了，跟vector基本一样。要想用list先加个头文件list。
*/


int MyStl::myList(void)
{
    int a[] = {1,2,3,4,5,5,6,7,7};
    size_t  count=sizeof(a)/sizeof(int);
    list<int> l1(a,a+count);
    showList(l1);
    list<int> l3(10);//将元素都初始化为0
    list<int> l4(5, 10);  //cha ru 5 ge 10
    showList(l4);

    return 0;
}

/*
map运用了哈希表地址映射的思想，也就是key-value的思想，来实现的。

首先给出map最好用也最最常用的用法例子，就是用字符串作为key去查询操作对应的value。

要使用map得先加个头文件map。
*/
/*
void showmap(map<string, int> v)
{
    for (map<string, int>::iterator it = v.begin(); it != v.end(); it++)
    {
        cout << it->first << "  " << it->second << endl;  //注意用法，不是用*it来访问了。first表示的是key，second存的是value
    }
    cout << endl;
}

int myMap(void)
{
    map<string, int> m1; //<>里的第一个参数表示key的类型,第二个参数表示value的类型
    m1["Kobe"] = 100;
    m1["James"] = 99;
    m1["Curry"] = 98;

    string s("Jordan");
    m1[s] = 90;

    cout << m1["Kobe"] << endl;
    cout << m1["Jordan"] << endl;
    cout << m1["Durant"] << endl; //不存在这个key，就显示0

    m1.erase("Curry");//通过关键字来删除
    showmap(m1);
    m1.insert(pair<string, int>("Harris", 89)); //也可以通过insert函数来实现增加元素
    showmap(m1);

    if (m1.count("Lee"))
	{
    	cout << "Lee is in m1!" << endl;
	}
	else
	{
    	cout << "Lee do not exist!" << endl;
	}

    m1.clear(); //清空全部
    return 0;
}
*/


//单向队列 queue支持 empty() size() front() back() push() pop()
//By MoreWindows(http://blog.csdn.net/MoreWindows)
int MyStl::myqueue(void)
{
    //可以使用list作为单向队列的容器，默认是使用deque的。
    queue<int, list<int> > a;
    queue<int>        b;
    int i;

    //压入数据
    for (i = 0; i < 10; i++)
    {
        a.push(i);
        b.push(i);
    }

    //单向队列的大小
    printf("%d %d\n", a.size(), b.size());

    //队列头和队列尾
    printf("%d %d\n", a.front(), a.back());
    printf("%d %d\n", b.front(), b.back());

    //取单向队列项数据并将数据移出单向队列
    while (!a.empty())
    {
        printf("%d ", a.front());
        a.pop();
    }
    putchar('\n');
    printf("%d ", a.size());
    putchar('\n');

    while (!b.empty())
    {
        printf("%d ", b.front());
        b.pop();
    }
    putchar('\n');
    printf("%d ", b.size());
    putchar('\n');
    return 0;
}


int MyStl::addCperson(CPerson* person)
{
    if (person != NULL)
    {
        mypersons.push_back(person);
    }
}
