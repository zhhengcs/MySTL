#include "pch.h"
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include "LinkedList.h"
using namespace std;


struct MyData
{
	int valueC;
	int valueD;
};
int compMyData(MyData*, MyData*);
//int compMyDataRev(MyData*, MyData*);
//int compMyDataD(MyData*, MyData*);

void test2();
void test1();
void test3();
void test4();
/*
int main()
{
	test1();
	//test2();
	//test3();
	//test4();
	return 0;
}
*/
void test1()
{
	int vals[] = {2,76,65,3,99,1,56, 34, 22, 11, 101, 6 };
	LinkedList<int> newList;
	for (int i = 0; i < 12; i++) {
		newList.push_back(&vals[i]);
	}
	for (LinkedList<int>::iterator it = newList.begin(); it != newList.end(); it++) {
		cout << (*it) << '\n';
	}

	newList.sort();
	cout << "After sort\n";
	for (LinkedList<int>::iterator it = newList.begin(); it != newList.end(); it++) {
		cout << (*it) << '\n';
	}
	cout << "finish" << endl;
}

void test2()
{
	LinkedList<MyData> newList;
	MyData myData;
	for(int i=0;i<20;++i)
	{
		myData.valueC = rand()%49;
		myData.valueD = rand() % 11;
		newList.push_back(&myData);
	}
	newList.sort(compMyData);
	for(LinkedList<MyData>::iterator it=newList.begin();it!=newList.end();it++)
	{
		cout << "Value C: " << (*it).valueC << " Value D: " << (*it).valueD << '\n';
	}
}

int compMyData(MyData* a, MyData* b)
{
	if (a->valueC < b->valueC) { return -1; }
	if (b->valueC < a->valueC) { return 1; }
	if (a->valueD < b->valueD) { return -1; }
	if (b->valueD < a->valueD) { return 1; }
	return 0;
}