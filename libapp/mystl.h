#ifndef MYSTL_H
#define MYSTL_H

#include <stdio.h>
#include <map>
#include <iostream>
#include <string>
#include <list>
#include <set>
#include <vector>
#include <queue>

using namespace std;

class MyStl
{
public:
	MyStl();
	~MyStl();
	static MyStl* getMyStl();
	int string_base(void);
	int myVector(void);
	int myqueue(void);
	int myList(void);
	int mySet(void);
	int getMyVector(std::vector<int> *v);
protected:
private:
	template <typename T>
	void showVector(vector<T> v);
	
	template <typename T>
	void showList(list<T> v);
	
	template <typename T>
	void showSet(set<T> v);
};
/*
int mySet(void);
int myList(void);
void showmap(map<string, int> v);
int myMap(void);
*/
#endif