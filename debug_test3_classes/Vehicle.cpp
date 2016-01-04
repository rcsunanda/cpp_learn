#include <iostream>

#include "Vehicle.h"


//*************************************************************************************************
Vehicle::Vehicle()
{

}


//*************************************************************************************************
Vehicle::~Vehicle()
{

}


//*************************************************************************************************
void Vehicle::init(std::string brand, const char* model, int capacity, double weight)
{
	m_brand = brand;
	m_model = model;
	m_engineCapacityCC = capacity;
	m_weight = weight;

	m_tyrePressure.push_back(32);
	m_tyrePressure.push_back(31);
	m_tyrePressure.push_back(30);
	m_tyrePressure.push_back(28);

	m_previousOwners["AAA"] = 2;
	m_previousOwners["XXX"] = 3;
	m_previousOwners["YYY"] = 5;
}


//*************************************************************************************************
void Vehicle::increaseCapacity(int x)
{
	std::cout << "increasing engine capacity by 1000, dummy x = " << x << std::endl;
	m_engineCapacityCC += 1000;
}


//*************************************************************************************************
void Vehicle::run()
{
	std::cout << "vehicle started running" << std::endl;

	std::cout << "brand: " << m_brand << std::endl;
	std::cout << "model: " << m_model << std::endl;
	std::cout << "engine capacity (CC): " << m_engineCapacityCC << std::endl;
	std::cout << "weight: " << m_weight << std::endl;

	std::cout << "vehicle finished running" << std::endl;
}

