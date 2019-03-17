#pragma once

#include <string>
#include <vector>
#include <unordered_map>


class Vehicle
{
public:
	
	Vehicle();
	~Vehicle();

	void init(std::string brand, const char* model, int capacity, double weight);

	void increaseCapacity(int x);

	void run();

private:
	
	std::string m_brand;
	const char* m_model;
	
	int m_engineCapacityCC;
	double m_weight;

	std::vector<int> m_tyrePressure;

	//key=name, value=owned years
	std::unordered_map<std::string, int> m_previousOwners;
};
