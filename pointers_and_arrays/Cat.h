#pragma once

#include <string>
#include <iostream>


class Cat
{
public:
	Cat(): m_age(-1), m_name("Kat") { std::cout << "Cat init" << std:: endl;}
	Cat(int age): m_age(age), m_name("Kat") {}
	Cat(int age, std::string name): m_age(age), m_name(name) {}
	
	~Cat() {}

	int getAge() { return m_age; }

private:
	int m_age;
	std::string m_name;
};
