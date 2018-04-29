#include "ClockBenchmark.h"
#include "hard_assert.h"
#include <cstdio>


ClockBenchmark::ClockBenchmark(int ensembleSize, int samples) :
	m_overheadTestStats(ensembleSize, samples),
	m_resolutionTestStats(ensembleSize, samples)
{
	m_ensembleSize = ensembleSize;
	m_sampleCount = samples;
}


void ClockBenchmark::runBenchmark()
{
	printf("Starting clock benchmark\n");
	m_clock.printClockInfo();
	printf("ensemble-size=%d; samples-count=%d\n", m_ensembleSize, m_sampleCount);

	std::string clockType;

	switch (CLOCK_TYPE)
	{
	case 1: clockType = "posix_monotonic"; break;
	case 2: clockType = "rdtscp"; break;
	case 3: clockType = "chrono"; break;
	case 4: clockType = "posix_realtime"; break;
	default: hard_assert(false);
	}
	
	warmupClock();

	measureOverhead();
	printf("\n---------- clock overhead test results ----------\n");
	m_overheadTestStats.printStats();
	
	std::string statsFilename = clockType + "_overhead_stats.csv";
	std::string bestWaveformFilename = clockType + "_overhead_best_waveform.csv";
	std::string worstWaveformFilename = clockType + "_overhead_worst_waveform.csv";

	m_overheadTestStats.writeStatsToFile(statsFilename);
	m_overheadTestStats.writeBestWaveformToFile(bestWaveformFilename);
	m_overheadTestStats.writeWorstWaveformToFile(worstWaveformFilename);

	measureResoluton();
	printf("\n---------- clock resolution test results ----------\n");
	m_resolutionTestStats.printStats();
	
	statsFilename = clockType + "_resolution_stats.csv";
	bestWaveformFilename = clockType + "_resolution_best_waveform.csv";
	worstWaveformFilename = clockType + "_resolution_worst_waveform.csv";

	m_resolutionTestStats.writeStatsToFile(statsFilename);
	m_resolutionTestStats.writeBestWaveformToFile(bestWaveformFilename);
	m_resolutionTestStats.writeWorstWaveformToFile(worstWaveformFilename);
}


void ClockBenchmark::warmupClock()
{
	for (int j = 0; j < 100; ++j)
	{
		uint64_t interval = 0;
		m_clock.setStartTS();
		m_clock.setEndTS();
		interval += m_clock.getMeasuredInterval();
		m_discardVal = interval;
	}
}


void ClockBenchmark::measureOverhead()
{
	for (int i = 0; i < m_ensembleSize; ++i)
	{
		for (int j = 0; j < m_sampleCount; ++j)
		{
			//two consecutive calls to timestamp functions

			m_clock.setStartTS();
			m_clock.setEndTS();

			uint64_t latency = m_clock.getMeasuredInterval();

			m_overheadTestStats.addLatencyValue(i, j, latency);
		}
	}

	m_overheadTestStats.calculateStats();
}

void ClockBenchmark::measureResoluton()
{
	for (int i = 0; i < m_ensembleSize; ++i)
	{
		for (int j = 0; j < m_sampleCount; ++j)
		{
			m_clock.setStartTS();

			//busyLoop(i);

			for (int k = 0; k < i; ++k)
				m_discardVal = 1;	//m_discardVal is declared volatile; prevents this loop from being optimized out

			m_clock.setEndTS();

			uint64_t latency = m_clock.getMeasuredInterval();

			m_resolutionTestStats.addLatencyValue(i, j, latency);
		}
	}

	m_resolutionTestStats.calculateStats();
}
