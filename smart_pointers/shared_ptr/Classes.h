#pragma once

#include <iostream>
#include <memory>


class Report
{
public:
	Report(int data) : m_data(data) { std::cout << "Constructor: data: " << data << std::endl; }
	~Report() { std::cout << "Destructor: data: " << m_data << std::endl; }

	int m_data;
	void print() { std::cout << "Report: data: " << m_data; }
};


class Message
{
public:
	std::shared_ptr<void> m_sharedVoidPtr;
};