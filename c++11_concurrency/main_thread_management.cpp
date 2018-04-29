//g++ -std=c++11 main_thread_management.cpp functions.cpp -o concurrency -lpthread

#include <iostream>
#include <thread>
#include <unistd.h>

#include "functions.h"
#include "Functor.h"
#include "RefFunctor.h"
#include "Cat.h"

int main(int argc, char** argv)
{
	std::cout << "Started C++11 concurrency: thread management program" << std::endl;

	//*********************************************************************************************

	do_work();	//This function runs in this thread (main)
	Functor f(5);
	f();	//The call operator runs in this thread (main)

	//*********************************************************************************************
	//Creating threads that run functions or functors, joining and detaching

	std::thread thread1{ do_work };	//Can pass just the function name
	std::thread thread2{ f };	//The object f is **copied** into the thread. The object must be callable

	std::thread thread3{ Functor() };	//Default constructed (in-place) Functor object is passed to the thread
	thread3.detach();	//This disassociates the underlying thread from the std::thread object
	//Therefore you can call detach() only once for a given thread (i.e. only thread3.joinable() is true) 
	//Usually, long running threads are detached (daemon threads), and they can be detached right after creating the std::thread object

	//Must call either join() or detach() before an std::thread object is destroyed
	//Because otherwise, std::thread's destructor calls std::terminate()

	thread1.join();
	thread2.join();

	//Once the associated thread in the std::thread object joins the main thread, join() cleans up any storage of the thread
	//So the std::thread object is no longer associated with the now-finished thread
	//Therefore you can call join() only once for a given thread (i.e. only thread1.joinable() is true)

	//Unlike detach(), join() can be called somewhere down the line
	//This means that the join() call is liable to be skipped if an exception is thrown (in the main thread here) before it is called
	//This can be solved by calling join() in the exception's catch block
	//Or by using a ThreadGaurd class and calling join in it's destructor (a ThreadGaurd object is created right after creating the thread, so it is destroyed in case of an exception)

	//*********************************************************************************************

	//Passing parameters: case 1
	std::thread thread4{ do_work_with_args, 5, 88.8, "hello" };	//The parameters are first copied (**passed by value**) to the std::thread's constructor, then a new thread is created
	//and then only those copies are given as parameters to the function --> this happens on the new thread
	thread4.detach();

	//Passing parameters: case 2: dangerous case: passing by reference or pointer to local variables (applicable to functors)
	{
		int x = 25;
		std::thread thread5{ RefFunctor{ x } };	//Set the member referece of the functor to refer to x
		thread5.detach();
	}	//x is now destroyed, but the m_numRef member variable (reference) of the functor still refers to this
	//Using that reference in the newly created thread5 results in undefined behaviour after x is destroyed
	//Make sure that any variables that the new thread uses last for its lifetime

	//Passing parameters: case 3: dangerous case: passing automatic variables (destroyed at end of scope)
	{
		char buffer[4] = { 'a', 'b', 'c' };
		std::thread thread6{ do_work_with_args, 5, 88.8, buffer };	//A pointer to the buffer is passed to std::thread constructor
		thread6.detach();
	}	//We could come here (buffer is destroyed) before 'buffer' is converted to a std::string on the new thread (thread6)
	//Results in undefined behaviour. Solution is to create the thread as follows
	//std::thread thread6{ do_work_with_args, 5, 88.8, std::string(buffer) }

	//Case 4: giving a class member function as the thread function
	Cat c1;
	std::thread thread7{ &Cat::processCat, &c1, 233 };
	thread7.join();

	//*********************************************************************************************
	//Transfering ownership of threads between std::thread objects
	//A single thread can be associated with only one std::thread object --> std::thread is no copyable, but only movable

	std::thread thread8{ do_work };	//Usual way to create thread

	std::thread thread9 = std::move(thread8);	//thread8 now has no associated thread. That thread is now associated with thread9

	thread8 = std::thread{ Functor{ 110 } };	//A temporary object is created and moved (implicitly and automatically) to thread8

	std::thread thread10;	//default constructed --> no associated thread;
	thread10 = std::thread{ do_work };	//associate a thread later

	//thread8 = std::move(thread9);	//will call std::terminate here because thread8 already has an associated thread

	thread8.join();
	thread9.join();
	thread10.join();

	//Because std::thread objects are movable in this manner, we can return them from functions or move them into function arguments
	
	/*
	//A function that returns an std::thread object
	std::thread f1()
	{
		return std::thread(some_function);
	}
	*/

	/*
	//A function that takes an std::thread object as an argument
	void f2(std::thread t);

	//call it as below

	f2(std::thread(some_function));	//Since a temporary is created, it is implicitly and automatically moved
	
	std::thread t(some_function);
	f2(std::move(t));	//Explicitly move the std::thread object into argument
	*/

	//To check the availability of harware concurrency (usually the number of cores)
	std::cout << "std::thread::hardware_concurrency() = " << std::thread::hardware_concurrency() << std::endl;

	//Thread identifier; this returns an object of type std::thread::id --> comparison, ordering (able to sort and put in containers) etc...
	std::cout << "main thread id: " << std::this_thread::get_id() << std::endl;

	sleep(1);	//trick to make sure any detached threads have finished running (works here only because threads are very small functions)

	return 0;
}