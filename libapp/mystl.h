#ifndef MYSTL_H
#define MYSTL_H

#include <map>
#include <iostream>
#include <string>
#include <list>
#include <set>
#include <vector>

using namespace std;
template <typename T>

int string_base(void);
void showvector(vector<T> v);
int myVector(void);
void showset(set<T> v);
int mySet(void);
void showlist(list<T> v);
int myList(void);
void showmap(map<string, int> v);
int myMap(void);

#endif