//Purpose: Check ability to save functors and function pointers as member variables

//g++ -std=c++11 function_objects_main.cpp -o function_objects

#include <iostream>
#include <functional>


class Thread
{
public:
	Thread() {}

	template <typename F>
	void setFunctor(F functor) { m_functor = functor; }

	void setFunctionPointer(void(*funcPointer)(void)) { m_functionPointer = funcPointer; }

	void callFunctor() { m_functor(); }
	void callFunctionPointer() { m_functionPointer(); }

private:
	std::function<void()> m_functor;
	void(*m_functionPointer)(void);
};


class MyFunctor
{
public:
	void operator()()
	{
		++m_data;
		std::cout << "new data: " << m_data << std::endl;
	}

private:
	int m_data = 0;
};


void someFunction()
{
	std::cout << "Some function" << std::endl;
}


int main(int argc, char** argv)
{
	Thread t;
	MyFunctor func;

	t.setFunctor(func);
	t.callFunctor();
	t.callFunctor();
	t.callFunctor();

	t.setFunctionPointer(someFunction);
	t.callFunctionPointer();
	t.callFunctionPointer();
	t.callFunctionPointer();

	return 0;
}