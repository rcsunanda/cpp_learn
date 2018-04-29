#include <iostream>
#include <thread>

#include "functions.h"


//*************************************************************************************************
void do_work()
{
	std::cout << "do_work() is running" << std::endl;
	std::cout << "do_work(): thread id: " << std::this_thread::get_id() << std::endl;
	std::cout << "do_work() exited" << std::endl;
}


//*************************************************************************************************
void do_work_with_args(int x, double d, std::string str)
{
	std::cout << "in do_work_with_args; x = " << x << ", d = " << d << ", str = " << str << std::endl;
}