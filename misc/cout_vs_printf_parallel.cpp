//Purpose: Check whether couts and printfs in prallel threads overlap

//g++ -std=c++11 cout_vs_printf_parallel.cpp -o cout_vs_printf_parallel -lpthread

#include <iostream>
#include <pthread.h>
#include <thread>
#include <unistd.h>


bool g_isJobComplete = false;


//*************************************************************************************************
void CreateThread(int coreID, void* (*fptr)(void*), void* data)
{
	pthread_t pthreadHandle;

	pthread_create(&pthreadHandle, NULL, fptr, data);

	if (coreID > -1)
	{
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(coreID, &cpuset);

		int ret = pthread_setaffinity_np(pthreadHandle, sizeof(cpu_set_t), &cpuset);

		if (ret != 0) // failed
			printf("Failed to set affinity=%d: err=%d\n", coreID, ret);
	}
}


//*************************************************************************************************
void* cout_thread(void* data)
{
	int* num_iter = static_cast<int*>(data);

	for (int i = 0; i < *num_iter; ++i)
	{
		std::cout << "XXXXXXXXXXX QQQQQQQQQ Very long message YYYYYYYYYYYYYYY QQQQQQQQQQ: This is cout: thread_id = " << std::this_thread::get_id() << std::endl;		
	}
	
	g_isJobComplete = true;
}


//*************************************************************************************************
void* printf_thread(void* data)
{
	int* num_iter = static_cast<int*>(data);
	
	size_t threadID = std::hash<std::thread::id>()(std::this_thread::get_id());

	for (int i = 0; i < *num_iter; ++i)
	{
		printf("XXXXXXXXXXX QQQQQQQQQ Very long message YYYYYYYYYYYYYYY QQQQQQQQQQ: This is printf: value = %d \n", threadID);
	}
	
	g_isJobComplete = true;
}


//*************************************************************************************************
int main(int argc, char** argv)
{
	if (argc < 4)
	{
		std::cout << "Usage: ./cout_vs_printf_parallel <core1> <core2> <num_iter>" << std::endl;
		return -1;
	}

	int core1 = atoi(argv[1]);
	int core2 = atoi(argv[2]);
	int num_iter = atoi(argv[3]);

	CreateThread(core1, printf_thread, &num_iter);
	CreateThread(core2, printf_thread, &num_iter);

	while(!g_isJobComplete)
	{
		sleep(1);
	}

	return 0;
}
