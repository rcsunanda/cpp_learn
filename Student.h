#pragma once

#include <string>
#include <cstring>
#include <iostream>

class Student
{
public:
	Student(): m_age(-1), m_name("John Doe"), m_city{"Colombo"}, m_country("Sri Lanka") {}
	Student(int age): m_age(age), m_name("John Doe") {}
	Student(int age, std::string name, const char* city): m_age(age), m_name(name)
	{
		strncpy(m_city, city, 20);
	}

	~Student() {}

	void printString(const char* str)
	{
		std::cout << "str = " << str << std::endl;
	}

	int getAge() { return m_age; }

	const char* getCity() { return m_city; }
	void setCity(const char* city) { strncpy(m_city, city, 20); }

private:
	const int m_ID = 10;
	int m_age = -1;

	std::string m_name;	//this is the preferred way to have string members
	
	char m_city[20];	//if std::string is not used, use this one
	
	const char* m_country = nullptr;	//this is not suitable, as it is only a pointer, and the string exists somewhere else in memory.
							//the ownership could be someone else's, and it could be destroyed without Student's knowledge

};
