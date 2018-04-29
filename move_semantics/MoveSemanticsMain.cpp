//g++ --std=c++11 -ggdb -fsanitize=address MoveSemanticsMain.cpp -o MoveSemantics


#include <cstdio>

#include "Animal.h"


void boo(Animal& a)
{
	printf("func=%s, line=%d, lvalue reference overload, a.age=%d, a.name=%s\n", __FUNCTION__, __LINE__, a.m_age, a.m_name.c_str());
}


void foo(Animal const & a)
{
	printf("func=%s, line=%d, const lvalue reference overload, a.age=%d, a.name=%s\n", __FUNCTION__, __LINE__, a.m_age, a.m_name.c_str());
}

void bar(Animal&& a)
{
	printf("func=%s, line=%d, rval reference, a.age=%d, a.name=%s\n", __FUNCTION__, __LINE__, a.m_age, a.m_name.c_str());
}

template <typename T>
void tmpFoo1(T& t)
{
	printf("func=%s, line=%d, templated lvalue reference overload, t.age=%d, t.name=%s\n", __FUNCTION__, __LINE__, t.m_age, t.m_name.c_str());
}

template <typename T>
void tmpFoo2(typename std::remove_reference<T>::type& t)
{
	printf("func=%s, line=%d, templated lvalue reference overload, t.age=%d, t.name=%s\n", __FUNCTION__, __LINE__, t.m_age, t.m_name.c_str());
}

int main()
{
	printf("MoveSemanticsMain started.\n");

	Animal a(10, "main-A");
	
	foo(a);

	Animal const & b = a;
	foo(b);
	//boo(b);	//error; cannor bind const-lval rference to non-const lval reference


	//foo(Animal(0, "temp"));	//error; cannot call non-const lval overload with temporary object

	foo(std::move(a));

	//bar(a);	error

	Animal d(11, "main-D");
	tmpFoo1(d);

	//tmpFoo2(d);	//error: because of std::remove_reference<T> in tmpFoo2, the function arguments have become a non-deducing context
	tmpFoo2<Animal>(d);	//must explictly specify the template parameter type


	printf("Test complete. Exiting\n");
	
	return 0;
}
