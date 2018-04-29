
//struct to keep high and low bits of the TSC register when read
struct CycleCounts
{
	unsigned int m_cycles_low;
	unsigned int m_cycles_high;
}

inline void get_start_TS(CycleCounts* startCounts)
{
	asm volatile ("CPUID\n\t"
	 "RDTSC\n\t"
	 "mov %%edx, %0\n\t"
	 "mov %%eax, %1\n\t": "=r" (startCounts->m_cycles_high), "=r" (startCounts->m_cycles_low)::
	"%rax", "%rbx", "%rcx", "%rdx");
}

inline void get_end_TS(CycleCounts* endCounts)
{
	asm volatile("RDTSCP\n\t"
	 "mov %%edx, %0\n\t"
	 "mov %%eax, %1\n\t"
	 "CPUID\n\t": "=r" (endCounts->m_cycles_high), "=r" (endCounts->m_cycles_low)::
	"%rax", "%rbx", "%rcx", "%rdx");
}

inline unsinged long long get_diff_cycle_count(CycleCounts* startCounts, CycleCounts* endCounts)
{
	unsinged long long startCount, endCount;
	
	startCount = ( ((unsinged long long)startCounts->m_cycles_high << 32) | startCounts->m_cycles_low );
	endCount = ( ((unsinged long long)endCounts->m_cycles_high << 32) | endCounts->m_cycles_low );
	
	return endCount - startCount;
}