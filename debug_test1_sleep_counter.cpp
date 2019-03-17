//g++ -g debug_test1_sleep_counter.cpp -o debug_test1_sleep_counter

#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[])
{
	std::cout << "**** started ****" << std::endl;

	int countI = 0;
	unsigned int countUI = 0;
	long countL = 0;

	while (true)
	{
		++countI;
		++countUI;
		++countL;

		sleep(3);
		std::cout << "...  " << std::flush;
	}

	return 0;
}
