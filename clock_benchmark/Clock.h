#pragma once

#include <cstdint>	//uint64_t
#include <ctime>	//timespec
#include <chrono>

//all clocks must implement printClockInfo(), setStartTS(), setEndTS(), getMeasuredInterval()
//we can enforce this interface with a pure abstract class but it's not done here to avoid abstraction cost (switch clock with macro in ClockBenchmark)

//-------------------------------------------------------------------------------------------------
class PosixMonotonicClock
{
public:

	void printClockInfo();
	void setStartTS();
	void setEndTS();
	uint64_t getMeasuredInterval();

private:
	uint64_t m_startTS = 0;
	uint64_t m_endTS = 0;

	struct timespec m_timeSpec;
};


//-------------------------------------------------------------------------------------------------
class RDTSCPClock
{
public:

	void printClockInfo();
	void setStartTS();
	void setEndTS();
	uint64_t getMeasuredInterval();

private:
	uint64_t m_startTS = 0;
	uint64_t m_endTS = 0;

	uint64_t m_startHigh = 0;
	uint64_t m_startLow = 0;
	uint64_t m_endHigh = 0;
	uint64_t m_endLow = 0;
};


//-------------------------------------------------------------------------------------------------
class ChronoClock
{
public:

	void printClockInfo();
	void setStartTS();
	void setEndTS();
	uint64_t getMeasuredInterval();

private:
	
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTS;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_endTS;
};


//-------------------------------------------------------------------------------------------------
class PosixRealTimeClock
{
public:

	void printClockInfo();
	void setStartTS();
	void setEndTS();
	uint64_t getMeasuredInterval();

private:

	uint64_t m_startTS = 0;
	uint64_t m_endTS = 0;

	struct timespec m_timeSpec;
};