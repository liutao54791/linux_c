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
#include "person.h"
#include "son.h"
#include "grandson.h"

using namespace std;

class CPerson;
class CSon;
class GrandSon;

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
	int addCperson(CPerson* person);

protected:
private:
	template <typename T>
	void showVector(vector<T> v);
	
	template <typename T>
	void showList(list<T> v);
	
	template <typename T>
	void showSet(set<T> v);

	std::vector<CPerson*> mypersons;
};
/*
int mySet(void);
int myList(void);
void showmap(map<string, int> v);
int myMap(void);
*/
#endif