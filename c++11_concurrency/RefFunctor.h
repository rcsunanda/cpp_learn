#pragma once

#include <iostream>


class RefFunctor
{
public:
	//RefFunctor(): {}
	RefFunctor(int& ref) : m_numRef{ ref } {}

	void operator() ()	const	//The call operator makes this class a functor
	{
		//++m_numRef;
		std::cout << "in RefFunctor::operator()(), m_numRef: " << m_numRef << std::endl;
	}

private:
	int& m_numRef;
};