#pragma once

#include "Clock.h"
#include "EnsembleStats.h"


class ClockBenchmark
{
public:
	ClockBenchmark(int ensembleSize, int samples);

	void runBenchmark();
	void warmupClock();
	void measureOverhead();
	void measureResoluton();

private:

	int m_ensembleSize;	//no. of waveforms
	int m_sampleCount;	//samples in one waveform
	volatile uint64_t m_discardVal;	//to prevent warmup code and busy loop in measureResoluton() from being optimized out

	EnsembleStats m_overheadTestStats;
	EnsembleStats m_resolutionTestStats;

#if CLOCK_TYPE==1
	PosixMonotonicClock m_clock;
#elif CLOCK_TYPE==2
	RDTSCPClock m_clock;
#elif CLOCK_TYPE==3
	ChronoClock m_clock;
#elif CLOCK_TYPE==4
	PosixRealTimeClock m_clock;
#endif

};
