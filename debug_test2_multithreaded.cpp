//g++ -g debug_test2_multithreaded.cpp -o debug_test2_multithreaded -lpthread

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <sys/prctl.h>


//*************************************************************************************************
long gettid()
{
	pthread_t ptid = pthread_self();
	long threadId = 0;
	memcpy(&threadId, &ptid, std::min(sizeof(threadId), sizeof(ptid)));
	return threadId;
}


//*************************************************************************************************
void* incrementer(void* data)
{
	std::cout << "**** incrementer thread started, thread ID: " << gettid() << " ****" << std::endl;
	prctl(PR_SET_NAME,"Incrementer Thread",0,0,0);
	
	int countI = 0;
	unsigned int countUI = 0;
	long long countL = 0;

	while (true)
	{
		++countI;
		++countUI;
		++countL;

		sleep(3);
		std::cout << "...  " << std::flush;
	}
}


//*************************************************************************************************
void* decrementer(void* data)
{
	std::cout << "**** decrementer thread started, thread ID: " << gettid() << " ****" << std::endl;
	prctl(PR_SET_NAME,"Decrementer Thread",0,0,0);

	int countBack = 0;

	while (true)
	{
		--countBack;

		sleep(3);
		std::cout << "'''  " << std::flush;
	}
}


//*************************************************************************************************
int main(int argc, char* argv[])
{
	std::cout << "**** main thread started, thread ID: " << gettid() << " ****" << std::endl;
	prctl(PR_SET_NAME,"Main Thread",0,0,0);

	int countI = 0;
	unsigned int countUI = 0;
	long long countL = 0;

	pthread_t incrementerThread;
	pthread_t decrementerThread;

	pthread_create(&incrementerThread, NULL, incrementer, NULL);
	pthread_create(&decrementerThread, NULL, decrementer, NULL);


	void* return_value = NULL;
	pthread_join(incrementerThread, &return_value);
	pthread_join(decrementerThread, &return_value);

	return 0;
}
