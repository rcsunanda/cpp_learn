#pragma once

#include <thread>

class Matrix;

//Simple usage of thread parallelism
//This class performs a matrix operation concurrently by executing it on two threads

class Calculator
{
public:

	void doConcurrentOperation(Matrix& matrix)
	{
		//split matrix into mat1 and mat2 (for data parallelism)

		std::thread t1(&Calculator::operate, this, mat1);
		std::thread t2(&Calculator::operate, this, mat2);

		t1.join();
		t2.join();

		//combine mat1 and mat2 if necessary
	}

private:

	int operate(Matrix& matrix)
	{
		//some calculation on matrix (assume changes are made to the input matrix itself)
	}
};