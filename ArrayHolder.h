#pragma once

#include <iostream>
#include "Cat.h"

constexpr int N = 4;

class ArrayHolder
{
public:
	ArrayHolder();
	~ArrayHolder();

private:
	//Case 4
	int arr1[N];	//array elements are garbage values --> initialize in c'tor
	Cat arr2[N];	//objects are initialized with default constructor
						//--> can use a different c'tor in initializer too (see the constructor)

	//Case 6
	int* p1;
	Cat* p2;	//p1, p2 pointers are initialized to garbage values


	//Case 7
	int* arr3[N];
	Cat* arr4[N];	//arr3, arr4 array elements are initialized to garbage values
						//--> create new objects and assign their addresses to array elements

	//Case 8
	int** pp1;
	Cat** pp2;	//pp1, pp2 pointers are initialized to garbage values
};


//*************************************************************************************************
ArrayHolder::ArrayHolder():
			arr1{0},
			arr2{Cat(43, "AliG"), Cat(55, "Darth"), Cat(12, "Vader"), Cat(15, "Fred")},	//can do this in a loop too

			p1{new int[6]},	//the 6 ints are zero-initialzed; the array size 6 can be determined at run-time
			p2{new Cat[3]},	//the 3 cats are initialized with default c'tor; the array size 3 can be determined at run-time

			arr3{new int(111), new int(222), new int(333), new int(444)},
			arr4{new Cat(43, "AliG"), new Cat(55, "Dath"), new Cat(12, "Vader"), new Cat(15, "Fred")},	//can do this in a loop too

			pp1{new int*[6]},
			pp2{new Cat*[3]}	//the pointer elements in the arrays pointed to by pp1 and pp2 are are initialized to zero
								//--> new objects must be created and their addresses must be assigned to the array elements (see below)
{
	//Creating new objects and storing their pointers in pp1 and pp2
	for (int i = 0; i < 6; ++i)
	{
		pp1[i] = new int(i);
	}

	for (int i = 0; i < 3; ++i)
	{
		pp2[i] = new Cat(i, "SomeCat");
	}
}


//*************************************************************************************************
ArrayHolder::~ArrayHolder()
{
	//deleting p1, p2
	delete[] p1;
	delete[] p2;

	//deleting arr3, arr4
	for (int i = 0; i < N; ++i)
	{
		delete arr3[i];
		delete arr4[i];
	}

	//deleting pp1, pp2
	for (int i = 0; i < 6; ++i)
	{
		delete pp1[i];
	}

	for (int i = 0; i < 3; ++i)
	{
		delete pp2[i];
	}

	delete[] pp1;
	delete[] pp2;
}
