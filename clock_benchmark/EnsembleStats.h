#pragma once

#include <cstdint>	//uint64_t
#include <string>


class EnsembleStats
{
public:
	EnsembleStats(int ensembleSize, int samples);

	void addLatencyValue(int waveformNo, int sampleNum, uint64_t latency);
	void calculateStats();
	void printStats();

	void writeStatsToFile(std::string filename);
	void writeBestWaveformToFile(std::string filename);
	void writeWorstWaveformToFile(std::string filename);
	void writeWaveformToFile(int waveformNum, std::string filename);

private:

	uint64_t getArrayVariance(uint64_t* arr, int arrSize);
	uint64_t getArrayMin(uint64_t* arr, int arrSize);
	uint64_t getArrayMax(uint64_t* arr, int arrSize);
	uint64_t getArrayMean(uint64_t* arr, int arrSize);

	int m_ensembleSize;	//no. of waveforms
	int m_sampleCount;	//samples in one waveform

	//main data set
	uint64_t** m_latencyValues;	//row = ensemble no, col = sample no

	//per-waveform stats
	uint64_t* m_means;	//mean of samples for each waveform
	uint64_t* m_variances;	//variance of samples for each waveform
	uint64_t* m_minValues;	//min of samples for each waveform
	uint64_t* m_maxDeviation;	//min max difference of each waveform
	
	//overall stats
	uint64_t m_meanOfVariances;	//mean of variances of all ensemables (m_variances)
	uint64_t m_absoluteMaxDev;	//maximum value amongst the max deviations of all the ensembles
	uint64_t m_varianceOfVariances;
	uint64_t m_varianceOfMinValues;
	uint64_t m_varianceOfMeans;

};
