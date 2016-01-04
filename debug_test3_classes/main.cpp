//g++ -g -std=c++11 main.cpp Vehicle.cpp -o debug_test3_classes

#include <iostream>
#include <vector>
#include <unistd.h>

#include "Vehicle.h"

int increment(int input)
{
	int count = 0;
	while (true)
	{
		++count;
		if (count > 1e3)
			break;
	}

	return ++input;
}


int decrement(int input)
{
	int count = 0;
	while (true)
	{
		++count;
		if (count > 5e2)
			break;
	}

	return --input;
}

int rest()
{

	int count = 0;
	while (true)
	{
		++count;
		sleep(3);
		if (count > 5)
			break;
	}
}

int main(int argc, char* argv[])
{
	std::cout << "**** started ****" << std::endl;
	std::cout << "process ID: " << getpid() << std::endl;

	int x = 266;
	int* px = new int(266);
	unsigned int number = 2147483914;
	double y = 88.78;

	long long p = 12;

	double z = y;

	const char* Cstring = "ABCD_XYZ";
	char* charPointer = "QQQAAA";
	char charArray[5] = {'1', '2', '\0', '4', '5'};
	int intArray[5] = {11, 22, 33, 44, 55};

	int* intPointer = intArray;

	Vehicle v1;
	v1.init("Toyota", "Carina", 1500, 1200);
	v1.run();

	Vehicle* v2 = new Vehicle();
	v2->init("Hondaaaaaaa", "Civiccccccccc", 500, 800);
	v2->run();

	/*
	std::vector<int> intVec1;
	x = intVec1.at(0);
	*/

	int countI = 0;

	int n = 10;
	for (int i = 0; i < n; ++i)
	{
		std::cout << i << "," << std::flush;
		int q = countI + 2;
		++q;
		q = q*i;
	}

	std::cout << std::endl;
	
	
	increment(10);
	decrement(15);

	return 0;
}
