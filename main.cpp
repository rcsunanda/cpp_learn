//g++ -std=c++11 -ggdb main.cpp Cat.cpp -o pointers_and_arrays

#include <iostream>
#include <cstdlib>

#include "Cat.h"

int main(int argc, char** argv)
{
	std::cout << "Starting program" << std::endl;
	
	//********************************************************************
	//Case 1: Simple pointer to object on stack

	int x = 10;
	int* p;

	//p = nullptr;
	p = &x;
	*p = 25;


	//********************************************************************
	//Case 2: Simple pointer to object on heap

	int* pHeapObject = new int(10);
	*pHeapObject = 25;


	//********************************************************************
	//Case 3: Pointer to pointer to object on heap (normal behavior)

	int* pHeapObject2 = new int(10);
	int** ppObject = &pHeapObject2;


	//********************************************************************
	//Case 4: Array on stack - check indexing and accessing via pointer arithmetic

	constexpr int N = 3;	//For correct behavior N must be const or constexpr (just int also works, but GDB doesn't recognize that stackArray is an array)
	int stackArray[N] = {10, 11, 12};
	int* pArr = stackArray;	//stackArray = &stackArray[0]
	std::cout << "stackArray = " << stackArray << ", &stackArray = " << &stackArray << ", pArr = " << pArr << std::endl;
	//Notice how --- stackArray = &stackArray = &stackArray[0]


	//********************************************************************
	//Case 5: Array on stack - runtime array size (VLA - variable length arrays)
	//--> this works if size >= 3 (3 is the size of the initialzer list): (except for issue of GDB not recognizing the array as an array)
	//This method is not recommended as it is not part of the C++ standard

	int size;
	if (argc > 1)
		size=atoi(argv[1]);

	int stackArray2[size] = {10, 11, 12};
	int* pArr2 = stackArray2;


	//********************************************************************
	//Case 6: Array on heap - Dynamically allocated contiguous memory block on heap (heap based array) - size calculated at run time because we're using new
	//(acting like an array; theoratically satisfies conditions for an array, but GDB doesn't recognize it as an array as it is technically a pointer)

	int* pBlock = nullptr;	//pointer to int (this is used to access the array we're creating on the heap)
	
	pBlock = new int[size];	//size is calculated at run-time; pointer pBlock points to the first element (int) of the newed memory block

	for (int i = 0; i < size; ++i)
	{
		pBlock[i] = i;
	}

	delete[] pBlock;


	//********************************************************************
	//Case 7: Array of Cat* (pointer to Cat) on stack - objects newed on heap
	
	Cat* catArr[5];

	//catArr[2]->getAge();	//Invalid (segfault) because pointers in catArr have garbage values; create Cat objects and assign their addresses

	for (int i = 0; i < 5; ++i)
	{
		catArr[i] = new Cat();
	}

	
	//********************************************************************
	//Case 8: Array of Cat* (pointer to Cat) on heap (compare with Case 6, where the array contains objects, not pointers)
	
	Cat** pCatPointerBlock = nullptr;	//pointer to Cat pointer (this is used to access the array of Cat pointers we're creating on the heap)

	pCatPointerBlock = new Cat*[size];	//size is calculated at run-time; pCatPointerBlock points to the first element (Cat pointer) of the newed memory block

	//pCatPointerBlock[2]->getAge(); ////Invalid (segfault) because pointers in the newed heap block have garbage values; create Cat objects and assign their addresses

	for (int i = 0; i < size; ++i)
	{
		pCatPointerBlock[i] = new Cat(i);
	}
	

	std::cout << "Exiting program" << std::endl;

	return 0;
}
