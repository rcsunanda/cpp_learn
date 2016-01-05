#pragma once

#include <string>


class Cat
{
public:
	Cat(): m_age(-1), m_name("Kat") {}
	Cat(int age): m_age(age), m_name("Kat") {}
	Cat(int age, std::string name): m_age(age), m_name(name) {}
	
	~Cat() {}

	int getAge() { return m_age; }

private:
	int m_age;
	std::string m_name;
};
