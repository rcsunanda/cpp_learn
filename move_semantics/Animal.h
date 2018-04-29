#pragma once

#include <string>


class Animal
{
public:

	Animal(int age, std::string name): 
		m_age(age), m_name(name)
	{
		printf("func=%s, line=%d, age=%d, name=%s\n", __FUNCTION__, __LINE__, m_age, m_name.c_str());
	}


//private:

	int m_age;
	std::string m_name;
};
