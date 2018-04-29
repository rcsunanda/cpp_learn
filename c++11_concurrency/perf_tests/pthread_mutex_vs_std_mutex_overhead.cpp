//This program compares performance of pthread_mutex_t and std::mutex
//TODO: include std::lock_gaurd<T>, std::unique_lock<T>, std::recursive_lock

//g++ -std=c++11 pthread_mutex_vs_std_mutex_overhead.cpp -o mutex_overhead -lpthread -O3
//g++ -std=c++11 pthread_mutex_vs_std_mutex_overhead.cpp -o mutex_overhead_westmere -lpthread -lrt -O3 -march=westmere -m64

//run this program pinned on a core with taskset



#include <iostream>
#include <utility>	//pair
#include <pthread.h>
#include <ctime>
#include <mutex>

//Globals
pthread_mutex_t g_pthreadMutex;
std::mutex g_stdMuetex;


//*************************************************************************************************
unsigned long gettimestamp()
{
	struct timespec tps;
	clock_gettime(CLOCK_MONOTONIC, &tps);
	//clock_gettime(CLOCK_MONOTONIC_COARSE, &tps);	
	return ((tps.tv_sec * 1e9) + tps.tv_nsec);
}


//following functions return average latency over N iterations

//*************************************************************************************************
double timeMeasurementOverhead(int N)
{
	unsigned long timeStamp, latency;
	unsigned long long sumOfLatency = 0;

	for (int i = 0; i < N; ++i)
	{
		timeStamp = gettimestamp();
		latency = gettimestamp() - timeStamp;
		sumOfLatency += latency;
	}
	
	return sumOfLatency / N;
}


//*************************************************************************************************
std::pair<double, double> pthreadMutexOverhead(int N)
{
	pthread_mutex_init(&g_pthreadMutex, NULL);

	unsigned long lockLatency, unlockLatency;
	unsigned long long sumOfLockLatency = 0, sumOfUnlockLatency = 0;

	for (int i = 0; i < N; ++i)
	{
		unsigned long beforeLock = gettimestamp();
		pthread_mutex_lock(&g_pthreadMutex);
		lockLatency = gettimestamp() - beforeLock;

		unsigned long beforeUnlock = gettimestamp();
		pthread_mutex_unlock(&g_pthreadMutex);
		unlockLatency = gettimestamp() - beforeUnlock;

		sumOfLockLatency += lockLatency;
		sumOfUnlockLatency += unlockLatency;
	}

	return std::make_pair(sumOfLockLatency / N, sumOfUnlockLatency / N);
}


//*************************************************************************************************
std::pair<double, double> stdMutexOverhead(int N)
{
	unsigned long lockLatency, unlockLatency;
	unsigned long long sumOfLockLatency = 0, sumOfUnlockLatency = 0;

	for (int i = 0; i < N; ++i)
	{
		unsigned long beforeLock = gettimestamp();
		g_stdMuetex.lock();
		lockLatency = gettimestamp() - beforeLock;

		unsigned long beforeUnlock = gettimestamp();
		g_stdMuetex.unlock();
		unlockLatency = gettimestamp() - beforeUnlock;

		sumOfLockLatency += lockLatency;
		sumOfUnlockLatency += unlockLatency;
	}

	return std::make_pair(sumOfLockLatency / N, sumOfUnlockLatency / N);
}


//*************************************************************************************************
int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << "Usage: taskset -c n1 ./mutex_overhead N \nN = no. of iterations, n1 = core number" << std::endl;
		return -1;
	}

	const int N = atoi(argv[1]);
	
	std::cout << "Mutex overhead program" << std::endl;

	double avgTimeMeasurementOverHead = timeMeasurementOverhead(N);

	double avgPthreadMutexLockOverHead = pthreadMutexOverhead(N).first;
	double avgPthreadMutexUnlockOverHead = pthreadMutexOverhead(N).second;

	double avgStdMutexLockOverHead = stdMutexOverhead(N).first;
	double avgStdMutexUnlockOverHead = stdMutexOverhead(N).second;


	std::cout << "\nTime measurement overhead average = " << avgTimeMeasurementOverHead << std::endl;

	std::cout << "\n----------------------- uncalibrated (raw) values -------------------------\n" << std::endl;

	std::cout << "Pthread lock overhead average = " << avgPthreadMutexLockOverHead << "\nPthread unlock overhead average = " << avgPthreadMutexUnlockOverHead << std::endl;
	std::cout << "std lock overhead average = " << avgStdMutexLockOverHead << "\nstd unlock overhead average = " << avgStdMutexUnlockOverHead << std::endl;

	std::cout << "\n------------------ values adjusted for time measurement overhead --------------------\n" << std::endl;
	
	std::cout << "Pthread lock overhead average = " << avgPthreadMutexLockOverHead - avgTimeMeasurementOverHead <<
		"\nPthread unlock overhead average = " << avgPthreadMutexUnlockOverHead - avgTimeMeasurementOverHead << std::endl;

	std::cout << "std lock overhead average = " << avgStdMutexLockOverHead - avgTimeMeasurementOverHead << 
		"\nstd unlock overhead average = " << avgStdMutexUnlockOverHead - avgTimeMeasurementOverHead << std::endl;

	std::cout << "\nExiting program" << std::endl;
}
