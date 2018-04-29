//On Ubuntu VM:
//g++ -std=c++11 std_atomic_vs_boost_atomic.cpp -o atomic_latency -I /mnt/hgfs/Ubuntu_VM_Shared_Folder/boost_1_55_0  -lpthread -O3

//On Fiber compiler machine:
//g++ -std=c++11 std_atomic_vs_boost_atomic.cpp -o atomic_latency -I $BOOST_ROOT -lpthread -lrt -O3 -march=westmere -m64


#include <boost/atomic.hpp>
#include <atomic>
#include <ctime>
#include <iostream>
#include <thread>


//*************************************************************************************************
unsigned long gettimestamp()
{
	struct timespec tps;
	clock_gettime(CLOCK_MONOTONIC, &tps);
	//clock_gettime(CLOCK_MONOTONIC_COARSE, &tps);	
	return ((tps.tv_sec * 1e9) + tps.tv_nsec);
}


//*************************************************************************************************
double stdAtomicLatency(int N, double* result = nullptr)	//second argument is for storing result when run as a thread
{
	std::atomic<int> stdInt{ 0 };

	unsigned long latency;
	unsigned long long sumOfLatency = 0;

	for (int i = 0; i < N; ++i)
	{
		unsigned long beforeLock = gettimestamp();
		++stdInt;
		latency = gettimestamp() - beforeLock;

		sumOfLatency += latency;
	}
	
	if (result)
		*result = sumOfLatency / N;

	return sumOfLatency / N;
}


//*************************************************************************************************
double boostAtomicLatency(int N, double* result = nullptr)
{
	boost::atomic<int> boostInt{ 0 };

	unsigned long latency;
	unsigned long long sumOfLatency = 0;

	for (int i = 0; i < N; ++i)
	{
		unsigned long beforeLock = gettimestamp();
		++boostInt;
		latency = gettimestamp() - beforeLock;

		sumOfLatency += latency;
	}

	if (result)
		*result = sumOfLatency / N;

	return sumOfLatency / N;
}


//*************************************************************************************************
int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << "Usage: ./atomic_latency N \nN = no. of iterations" << std::endl;
		return -1;
	}

	const int N = atoi(argv[1]);

	std::cout << "Atomic latency test program started" << std::endl;

	//Same thread incrementing

	double avgStdAtomicLatency = stdAtomicLatency(N);
	double avgBoostAtomicLatency = boostAtomicLatency(N);
	
	std::cout << "number of iterations = " << N << std::endl;
	std::cout << "std::atomic latency average = " << avgStdAtomicLatency << std::endl;
	std::cout << "boost::atomic latency average = " << avgBoostAtomicLatency << std::endl;

	//Incrementing in 2 parallel threads

	//std::atomic

	double avgStdAtomicThread1 = 0;
	double avgStdAtomicThread2 = 0;

	std::thread stdThread1{ stdAtomicLatency, N, &avgStdAtomicThread1 };
	std::thread stdThread2{ stdAtomicLatency, N, &avgStdAtomicThread2 };

	stdThread1.join();
	stdThread2.join();

	std::cout << "std::atomic latency averages: thread 1: " << avgStdAtomicThread1 << ", thread 2: " << avgStdAtomicThread2 << std::endl;

	//boost::atomic

	double avgBoostAtomicThread1 = 0;
	double avgBoostAtomicThread2 = 0;

	std::thread boostThread1{ boostAtomicLatency, N, &avgBoostAtomicThread1 };
	std::thread boostThread2{ boostAtomicLatency, N, &avgBoostAtomicThread2 };

	boostThread1.join();
	boostThread2.join();

	std::cout << "boost::atomic latency averages: thread 1: " << avgBoostAtomicThread1 << ", thread 2: " << avgBoostAtomicThread2 << std::endl;

	return 0;
}