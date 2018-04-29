//g++ -std=c++11 main_atomic_int.cpp -o atomic_int -lpthread

#include <thread>
#include <mutex>
#include <atomic>
#include <iostream>


//int gCounter = 0;
std::atomic<int> gCounter{0};
std::mutex g_CounterMutex;
std::mutex g_printMutex;

constexpr int N = 10000;

void increment()
{
	for (int i = 0; i < N; ++i)
	{
		//g_CounterMutex.lock();
		++gCounter;
		//std::cout << gCounter << std::endl;
		//g_CounterMutex.unlock();
	}
}


int getNext()
{
	return ++gCounter;
}


void getAndPrint()
{
	int* storage = new int[N];

	for (int i = 0; i < N; ++i)
	{
		storage[i] = gCounter;
	}

	//Print
	g_printMutex.lock();
	for (int i = 0; i < N; ++i)
	{
		std::cout << storage[i] << std::endl;
	}
	g_printMutex.unlock();

	delete[] storage;
}



int main(int argc, char** argv)
{
	std::thread t1{ increment };
	std::thread t2{ increment };
	std::thread t3{ increment };
	std::thread t4{ getAndPrint };

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	std::cout << "gCounter final value = " << gCounter << std::endl;
}