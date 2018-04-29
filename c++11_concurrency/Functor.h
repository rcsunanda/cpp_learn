#pragma once

#include <iostream>


class Functor
{
public:
	Functor() : m_number(0) {}
	Functor(int n) : m_number(n) {}
	
	void operator() ()	//The call operator makes this class a functor
	{
		++m_number;
		std::cout << "in Funtor::operator()(), m_number: " << m_number << std::endl;
	}

private:
	int m_number;
};