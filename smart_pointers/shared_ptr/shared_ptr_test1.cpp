//g++ -std=c++11 shared_ptr_test1.cpp -o shared_ptr_test1

#include "Classes.h"

#include <iostream>
#include <memory>

//using std;

int main()
{
	std::cout << "---Started shared_ptr---" << std::endl;

	std::shared_ptr<Report> reportSP1(new Report(1));
	std::shared_ptr<Report> reportSP2(new Report(2));
	std::shared_ptr<Report> reportSP3 = reportSP1;

	
	//reportSP1 = reportSP2;	//Ownership of Report(2) is shared with reportSP1. Before that, reportSP1's refcount is decremented

	std::cout << "---Finished shared_ptr---" << std::endl;


	return 0;
}