//CLOCK_TYPE=1 for clock_gettime(CLOCK_MONOTONIC)
//CLOCK_TYPE=2 for RDTSCP assembly call
//CLOCK_TYPE=3 for std::chrono::high_resolution_clock
//CLOCK_TYPE=4 for clock_gettime(CLOCK_REALTIME)

#include <cstdio>
#include <pthread.h>
#include "ClockBenchmark.h"
#include "hard_assert.h"

#include <ctime>	//timespec

uint64_t gettimestamp()
{
	struct timespec tps;
	clock_gettime(CLOCK_MONOTONIC, &tps);
	//clock_gettime(CLOCK_MONOTONIC_COARSE, &tps);	
	return ((tps.tv_sec * 1e9) + tps.tv_nsec);
}


int main(int argc, char** argv)
{

#ifdef NDEBUG
	const char *zBuildType = "RELEASE";
#else
	const char *zBuildType = "DEBUG";
#endif

	printf("clock_benchmark: build: %s; %s; (%s)\n", __DATE__, __TIME__, zBuildType);
	printf("Usage: clock_benchmark <core-id> <ensemble-size> <sample-count>\n");

	hard_assert(argc == 4);

	int core = atoi(argv[1]);
	int ensembleSize = atoi(argv[2]);
	int sampleCount = atoi(argv[3]);

	//set affinity
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(core, &cpuset);
	int result = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
	hard_assert(result == 0);	//failed to set affinity


	//start test

	printf("clock_type=%d; cpu=%d\n\n", CLOCK_TYPE, core);

	ClockBenchmark clockBench(ensembleSize, sampleCount);
	clockBench.runBenchmark();

	/* //test code
	EnsembleStats x(10, 20);

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			x.m_latencyValues[i][j] = i + j;
			printf("%llu, ", x.m_latencyValues[i][j]);
		}
		printf("\n");
	}

	x.calculateStats();
	x.printStats();

	for (int i = 0; i < 10; ++i)
	{
		printf("waveform=%d, min=%llu; mean=%llu; max-dev=%llu; var=%llu\n", 
			i, x.m_minValues[i], x.m_means[i], x.m_maxDeviation[i], x.m_variances[i]);
	}
	*/

	return 0;
}
