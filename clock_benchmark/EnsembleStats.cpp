#include "EnsembleStats.h"
#include "hard_assert.h"
#include <cstdio>
#include <fstream>


EnsembleStats::EnsembleStats(int ensembleSize, int samples)
{
	m_ensembleSize = ensembleSize;
	m_sampleCount = samples;

	//allocate memory to arrays

	m_latencyValues = new uint64_t*[ensembleSize];

	for (int i = 0; i < ensembleSize; ++i)
	{
		m_latencyValues[i] = new uint64_t[samples];
	}

	m_means = new uint64_t[ensembleSize];
	m_variances = new uint64_t[ensembleSize];
	m_minValues = new uint64_t[ensembleSize];
	m_maxDeviation = new uint64_t[ensembleSize];
}

void EnsembleStats::addLatencyValue(int waveformNum, int sampleNum, uint64_t latency)
{
	hard_assert(waveformNum < m_ensembleSize);
	hard_assert(sampleNum < m_sampleCount);

	m_latencyValues[waveformNum][sampleNum] = latency;
}

void EnsembleStats::calculateStats()
{
	for (int i = 0; i < m_ensembleSize; ++i)
	{
		m_means[i] = getArrayMean(m_latencyValues[i], m_sampleCount);
		m_variances[i] = getArrayVariance(m_latencyValues[i], m_sampleCount);
		m_minValues[i] = getArrayMin(m_latencyValues[i], m_sampleCount);
		m_maxDeviation[i] = getArrayMax(m_latencyValues[i], m_sampleCount) - getArrayMin(m_latencyValues[i], m_sampleCount);
	}

	m_meanOfVariances = getArrayMean(m_variances, m_ensembleSize);
	m_absoluteMaxDev = getArrayMax(m_maxDeviation, m_ensembleSize);
	m_varianceOfVariances = getArrayVariance(m_variances, m_ensembleSize);
	m_varianceOfMinValues = getArrayVariance(m_minValues, m_ensembleSize);
	m_varianceOfMeans = getArrayVariance(m_means, m_ensembleSize);
}

void EnsembleStats::printStats()
{
	printf("mean-of-variances=%llu\n", m_meanOfVariances);
	printf("absolute-max-deviation=%llu\n", m_absoluteMaxDev);
	printf("variance-of-variances=%llu\n", m_varianceOfVariances);
	printf("variance-of-min-values=%llu\n", m_varianceOfMinValues);
	printf("variance-of-means=%llu\n", m_varianceOfMeans);
}

void EnsembleStats::writeStatsToFile(std::string filename)
{
	std::ofstream fileStream;
	fileStream.open(filename.c_str());

	fileStream << "#waveform-no, variance, min, mean\n";

	for (int i = 0; i < m_ensembleSize; ++i)
	{
		std::string waveformNum = std::to_string(i);
		std::string variance = std::to_string(m_variances[i]);
		std::string min = std::to_string(m_minValues[i]);
		std::string mean = std::to_string(m_means[i]);

		fileStream << waveformNum << ", " << variance << ", " << min << ", " << mean << "\n";
	}

	fileStream.close();
}

void EnsembleStats::writeBestWaveformToFile(std::string filename)
{
	//best waveform = waveform with minimum max-deviation

	uint64_t min = getArrayMin(m_maxDeviation, m_ensembleSize);

	for (int i = 0; i < m_ensembleSize; ++i)
	{
		if (m_maxDeviation[i] == min)	//found a waveform with minimum max-deviation
		{
			printf("Writing best waveform no: %d to file: %s; waveform-variance: %llu\n", i, filename.c_str(), m_variances[i]);
			writeWaveformToFile(i, filename);
			break;
		}
	}
}

void EnsembleStats::writeWorstWaveformToFile(std::string filename)
{
	//worst waveform = waveform with maximum max-deviation

	uint64_t max = getArrayMax(m_maxDeviation, m_ensembleSize);

	for (int i = 0; i < m_ensembleSize; ++i)
	{
		if (m_maxDeviation[i] == max)	//found a waveform with maximum max-deviation
		{
			printf("Writing worst waveform no: %d to file: %s; waveform-variance: %llu\n", i, filename.c_str(), m_variances[i]);
			writeWaveformToFile(i, filename);
			break;
		}
	}
}

void EnsembleStats::writeWaveformToFile(int waveformNum, std::string filename)
{
	uint64_t* waveform = m_latencyValues[waveformNum];

	std::ofstream fileStream;
	fileStream.open(filename.c_str());

	fileStream << "#sample-no, latency\n";

	for (int j = 0; j < m_sampleCount; ++j)
	{
		std::string sampleNum = std::to_string(j);
		std::string latency = std::to_string(waveform[j]);

		fileStream << sampleNum << ", " << latency << "\n";
	}

	fileStream.close();
}

uint64_t EnsembleStats::getArrayVariance(uint64_t* arr, int arrSize)
{
	uint64_t mean = getArrayMean(arr, arrSize);
	uint64_t squareSum = 0;

	for (int j = 0; j < arrSize; ++j)
	{
		squareSum += (arr[j] - mean) * (arr[j] - mean);
	}

	return squareSum/arrSize - 1;
}

uint64_t EnsembleStats::getArrayMin(uint64_t* arr, int arrSize)
{
	uint64_t min = UINT64_MAX;

	for (int j = 0; j < arrSize; ++j)
	{
		if (arr[j] < min)
			min = arr[j];
	}

	return min;
}

uint64_t EnsembleStats::getArrayMax(uint64_t* arr, int arrSize)
{
	uint64_t max = 0;

	for (int j = 0; j < arrSize; ++j)
	{
		if (arr[j] > max)
			max = arr[j];
	}

	return max;
}

uint64_t EnsembleStats::getArrayMean(uint64_t* arr, int arrSize)
{
	uint64_t sum = 0;

	for (int j = 0; j < arrSize; ++j)
	{
		sum += arr[j];
	}

	uint64_t mean = sum / arrSize;
	return mean;
}
