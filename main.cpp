//g++ -std=c++11 -ggdb main.cpp Cat.cpp -o pointers_and_arrays

#include <iostream>
#include <cstdlib>

#include "Cat.h"
#include "Student.h"
#include "ArrayHolder.h"

int main(int argc, char** argv)
{
	std::cout << "Entering program" << std::endl;
	
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
	//Case 4: Array on stack - indexing and accessing via pointer arithmetic

	constexpr int N = 3;	//For correct behavior N must be const or constexpr (just int also works, but GDB doesn't recognize that stackArray is an array)
	int stackArray[N] = {10, 11, 12};

	int* pointerToFirstElement = &stackArray[0];	//A
	int* decayedPointerToFirstElement = stackArray;	//B
	int (*pointerToArray)[N] = &stackArray;	//C
	
	//Technically, pointer to an array is C, but normally when we say pointer to an array, we mean pointer to the first element as in A or B
	//When A or B are subscripted or incremented, we get the elements of the array
	//When C is subscripted or incremented, we get an entire array of int with size N --> useful only for arrays of arrays

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
	//Seen in classes, but not suitable if the object is large (unlike int), as although they're created in the heap, this needs a large chunk of contiguous heap memory

	Cat* pBlock = new Cat[size];	//pointer to Cat (this is used to access the array we're creating on the heap)
	
	//size is calculated at run-time; pointer pBlock points to the first element (Cat) of the newed memory block
	//default constructor is called for each element when allocating a memory block of objects like this

	for (int i = 0; i < size; ++i)
	{
		//std::cout << "Cat # " << i << ", age: " << pBlock[i].getAge() << std::endl;	//valid because the array contains default-constructed Cats
	}

	delete[] pBlock;	//Cat Destructor is run on each object, and then the memory block is released


	//********************************************************************
	//Case 7: Array of Cat* (pointer to Cat) on stack - objects are newed on heap
	//Can be used in classes, but if the the array size is large and the class object is created on the stack, it might be a problem
	//		(even though the array contains pointers and actual objects are on the heap)
	
	Cat* catPointerArray[5]; //Array of pointers on stack

	//catPointerArray[2]->getAge();  //Invalid (segfault) because pointers in catArr have garbage values; create Cat objects and assign their addresses

	for (int i = 0; i < 5; ++i)
	{
		catPointerArray[i] = new Cat();
	}

	//After usage call delete on each pointer individually

	for (int i = 0; i < 5; ++i)
	{
		delete catPointerArray[i];
	}

	
	//********************************************************************
	//Case 8: Array of Cat* (pointer to Cat) on heap (compare with Case 6, where the array contains objects, not pointers)
	//This is also commonly seen in classes, very suitable as the class contains only a single pointer so class objects can be created on stack with no issue (unlike Case 7),
	//			and the array contains pointers so Cat object size doesn't matter (unlike Case 6)
	
	Cat** pCatPointerBlock = nullptr;	//pointer to Cat pointer (this is used to access the array of Cat pointers we're creating on the heap)

	pCatPointerBlock = new Cat*[size];	//size is calculated at run-time; pCatPointerBlock points to the first element (Cat pointer) of the newed memory block

	//pCatPointerBlock[2]->getAge(); ////Invalid (segfault) because pointers in the newed heap block have garbage values; create Cat objects and assign their addresses

	for (int i = 0; i < size; ++i)
	{
		pCatPointerBlock[i] = new Cat(i);
	}

	//After usage call delete on each pointer individually, and also deallocate the memory block

	for (int i = 0; i < 5; ++i)
	{
		delete pCatPointerBlock[i];
	}
	
	delete[] pCatPointerBlock;	//deallocate the memory block

	//********************************************************************
	//Case 9: Const pointer - modifying an object via a const pointer (not allowed)
	//Const array - modifying objects in a const array (not allowed)

	const int* pInt = new int(25);
	//*pInt = 222;	//not allowed (does not compile), because we cannot modify the integer via pInt (const int*)

	//if you really want to modify, you need a const_cast
	int* nonConstPint = const_cast<int*>(pInt);
	*nonConstPint = 333;

	const int constIntArr[] = {1, 2, 3, 4, 5};
	//constIntArr[0] = 111;	//not allowed (does not compile), because element type is const int
	//int* ptr = constIntArr; //not allowed (does not compile), invalid conversion from ‘const int*’ to ‘int*’


	//********************************************************************
	//Case 10: String literals

	/////////////////////////////////////////As arrays

	//As const char[] array (array of null-terminated characters)
	const char str1[] = "Yohn Rajendra";	//Special way of initializing a character array (compiler adds a null character to the end)
								//&str1 == (const char (*)[5]) 0xbffff572 AND ALSO, p &str1[0] == 0xbffff572 "John"
								//--->This array is on the stack --> check with destruction --> verified with following code
	
	//When initializing as above, if you're giving an array size, make sure that size = length + 1 (for null-character)
	{
		const char stackCharArr[] = "CooooooL";
		std::cout << "dummy" << std::endl;
	}
	
	//str1[0] = 'K';	//not allowed (constness), even if modified with a const_cast, undeffined behaviour

	const char str2[] = {'J', 'o', 'h', 'n', '\0'};	//same as above. But memory location --> &str2 = (const char (*)[5]) 0x8049055 <str2> ---> (on .rodata)

	//C string literal as char[] array
	char str3[] = "Mary";
	str3[0] = 'K';	//Perfectly fine to modify elements in this array. see note in memory placement

	char strX[] = {'Q', 'W', 'E', 'R', '\0'}; //Compiler treats this as an array of type char rather than a string literal, and places it on the stack


	//Note: String literals are not modifiable. They may be placed in read-only memory such as .rodata

	/////////////////////////////////////////As pointers

	//As const char* (pointer to first element in array of null-terminated characters) --> //Compare this with Case 4 and 6 (it's the same)
	const char* str4 = "Jenny";	//str4 = 0x804903f "Jenny"

	//C string literal as char* --> valid in C++03 (due to a special rule in the standard), invalid in C++11 (however compiler may allow it and warn that it's deprecated)
	char* str5 = "Dean";
	//str5[0] = 'Q';	//Undefined behavior --> gives segfault as it's trying to modify data in a read-only memory section (.rodata)

	//Invalid attempt to modify a string literal (String literals are not modifiable)
	const char* str6 = "Martin";
	str6 = "Frank";	//This compiles because we're modifying the pointer (address of location it's pointing to), and not the pointed value (which is const char)
					//It modifies the pointer to point to a separate string literal in a different memory location (possibly .rodata section) that was created at compile-time
					//We loose the pointer to the earlier string "Martin" (it is not destroyed as string literals have static duration)
					//This expression can fool you to think that you're modifying the original string "Martin" to "Frank", and this could lead to a large .rodata section, which is not desirable

	str6 = argv[0];
	
	/////////////////////////////////////////Memory placement of string literals (gcc 4.9)
	
	//Following on .rodata
	//const char* str = "AAA"
	//char* str = "QQQ"
	//const char str[] = {'W', 'W', 'W', '\0'}


	//Following on stack
	//const char str[] = "WWW"
	//char str[] = "RRR"
	//char str[] = {'W', 'W', 'W', '\0'}	//As opposed to const version

	//These depends on compiler, executable format and platform.
	//General rule: if you want mutable string literals, use non-const character arrays, (char mutableString[])

	/////////////////////////////////// where will a string literal assigned at run time go? probably not .rodata????


	//********************************************************************
	//Case 11: Strings in classes

	Student s1;
	s1.printString("POLIA");
	s1.printString(argv[2]);

	Student s2(44, "Moppy", "Islamabad");

	//********************************************************************
	//Case 12: Array initialization
	
	double aArr[5];
	double bArr[5] = {};
	double cArr[5] = {0.0};
	double dArr[5] = {1.5};

	double eArr[5];
	std::fill(eArr, eArr+5, 6.6);

	double fArr[5];
	std::fill_n(fArr, 5, 7.7);

	//********************************************************************
	//Case 13: Arrays in classes

	ArrayHolder holder;

	ArrayHolder* pHolder = new ArrayHolder();

	delete pHolder;

	std::cout << "Exiting program" << std::endl;

	return 0;
}
