#include "Clock.h"
#include "hard_assert.h"
#include <cstdio>
#include <iostream>


void PosixMonotonicClock::printClockInfo()
{
	struct timespec res;
	clock_getres(CLOCK_MONOTONIC, &res);
	uint64_t resolutionNS = res.tv_sec * 1e9 + res.tv_nsec;
	printf("clock-name=PosixMonotonicClock; nominal-resolution-ns=%llu\n", resolutionNS);
}

void PosixMonotonicClock::setStartTS()
{
	clock_gettime(CLOCK_MONOTONIC, &m_timeSpec);
	m_startTS = m_timeSpec.tv_sec * 1e9 + m_timeSpec.tv_nsec;
}

void PosixMonotonicClock::setEndTS()
{
	clock_gettime(CLOCK_MONOTONIC, &m_timeSpec);
	m_endTS = m_timeSpec.tv_sec * 1e9 + m_timeSpec.tv_nsec;
}

uint64_t PosixMonotonicClock::getMeasuredInterval()
{
	hard_assert(m_endTS >= m_startTS);
	//printf("PosixMonotonicClock::getMeasuredInterval; interval=%llu\n", m_endTS - m_startTS);
	return m_endTS - m_startTS;
}


//-------------------------------------------------------------------------------------------------

void RDTSCPClock::printClockInfo()
{
	printf("clock-name=RDTSCPClock\n");
}

//following functions work on Ubuntu 32-bit, but not on SUSE 64-bit

//void RDTSCPClock::setStartTS()
//{
//	asm volatile ("CPUID\n\t"
//		"RDTSC\n\t"
//		"mov %%edx, %0\n\t"
//		"mov %%eax, %1\n\t": "=g" (m_startHigh), "=r" (m_startLow)::
//		"%rax", "%rbx", "%rcx", "%rdx");
//}
//
//void RDTSCPClock::setEndTS()
//{
//	asm volatile("RDTSCP\n\t"
//		"mov %%edx, %0\n\t"
//		"mov %%eax, %1\n\t"
//		"CPUID\n\t": "=g" (m_endHigh), "=r" (m_endLow)::
//		"%rax", "%rbx", "%rcx", "%rdx");
//}


//Following functions work on SUSE (64-bit)
void RDTSCPClock::setStartTS()
{
	asm volatile (
		"CPUID\n"
		"RDTSC"
		: "=a"(m_startLow), "=d"(m_startHigh) /* outputs */
		: "a"(0)             /* inputs */
		: "%rbx", "%rcx");     /* clobbers*/
}

void RDTSCPClock::setEndTS()
{
	asm volatile (
		"RDTSCP"
		: "=a"(m_endLow), "=d"(m_endHigh) /* outputs */
		: "a"(0)             /* inputs */
		: "%rbx", "%rcx");	/* clobbers*/
	
	asm volatile ("CPUID");
}

uint64_t RDTSCPClock::getMeasuredInterval()
{
	m_startTS = ((uint64_t)m_startHigh << 32) | m_startLow;
	m_endTS = ((uint64_t)m_endHigh << 32) | m_endLow;
	//printf("RDTSCPClock::getMeasuredInterval; interval=%llu\n", m_endTS - m_startTS);
	return m_endTS - m_startTS;
}


//-------------------------------------------------------------------------------------------------

void ChronoClock::printClockInfo()
{
	std::chrono::duration<double, std::nano> resolution = std::chrono::high_resolution_clock::duration{ 1 };

	std::cout << "clock-name=C++11-chrono-high_resolution_clock; " <<
		"nominal-resolution-ns=" << resolution.count() << "; is-clock-monotonic=" << std::chrono::high_resolution_clock::is_steady << std::endl;
}

void ChronoClock::setStartTS()
{
	m_startTS = std::chrono::high_resolution_clock::now();
}

void ChronoClock::setEndTS()
{
	m_endTS = std::chrono::high_resolution_clock::now();
}

uint64_t ChronoClock::getMeasuredInterval()
{
	std::chrono::duration<double> diffSeconds = m_endTS - m_startTS;

	//std::cout << "elapsed time: " << diff.count() << "seconds\n";
	
	uint64_t diffNS = std::chrono::duration_cast<std::chrono::nanoseconds>(diffSeconds).count();

	//printf("ChronoClock::getMeasuredInterval; interval=%llu\n", diffNS);
	
	return diffNS;
}


//-------------------------------------------------------------------------------------------------

void PosixRealTimeClock::printClockInfo()
{
	struct timespec res;
	clock_getres(CLOCK_REALTIME, &res);
	uint64_t resolutionNS = res.tv_sec * 1e9 + res.tv_nsec;
	printf("clock-name=PosixRealTimeClock; nominal-resolution-ns=%llu\n", resolutionNS);
}

void PosixRealTimeClock::setStartTS()
{
	clock_gettime(CLOCK_REALTIME, &m_timeSpec);
	m_startTS = m_timeSpec.tv_sec * 1e9 + m_timeSpec.tv_nsec;
}

void PosixRealTimeClock::setEndTS()
{
	clock_gettime(CLOCK_REALTIME, &m_timeSpec);
	m_endTS = m_timeSpec.tv_sec * 1e9 + m_timeSpec.tv_nsec;
}

uint64_t PosixRealTimeClock::getMeasuredInterval()
{
	hard_assert(m_endTS >= m_startTS);
	//printf("PosixRealTimeClock::getMeasuredInterval; interval=%llu\n", m_endTS - m_startTS);
	return m_endTS - m_startTS;
}