//g++ -std=c++11 main_locks.cpp -o concurrency -lpthread

#include <iostream>
#include <thread>

#include "ThreadSafeClass.h"

//#include <unistd.h>


int main(int argc, char** argv)
{
	std::cout << "Started C++11 concurrency: locks program" << std::endl;

	ThreadSafeClass TSObject;

	std::thread thread1{ &ThreadSafeClass::print_1, &TSObject };	//run TSObject.getAndPrint() in thread1
	std::thread thread2{ &ThreadSafeClass::print_1, &TSObject };	//run TSObject.getAndPrint() in thread2

	thread1.join();
	thread2.join();

	std::thread thread3{ &ThreadSafeClass::print_2, &TSObject };	//run TSObject.getAndPrint() in thread1
	std::thread thread4{ &ThreadSafeClass::print_2, &TSObject };	//run TSObject.getAndPrint() in thread2

	thread3.join();
	thread4.join();

	return 0;
}