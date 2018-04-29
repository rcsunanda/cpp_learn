#pragma once

#include <iostream>
#include <vector>

class Cat
{
public:
	Cat() {}
	~Cat() {}

	void processCat(int n)
	{
		std::cout << "Cat's age: " << m_age << ", n = " << n << std::endl;
	}

private:
	int m_age;
};