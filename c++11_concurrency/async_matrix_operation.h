#pragma once

#include <thread>

class Matrix;

//This class performs a matrix operation concurrently by executing it as two packaged_tasks (on two threads)
//Results of operations are retrieved from the threads via the future associated with a packaged_task

class Calculator
{
public:

	void doConcurrentOperation(Matrix& matrix)
	{
		//split matrix into mat1 and mat2 (for data parallelism)

		future<double> future1 = std::async(&Calculator::operate, this, mat1); // run task potentially asynchronously, and get associated future
		future<double> future2 = std::async(&Calculator::operate, this, mat2); // run task potentially asynchronously, and get associated future

		Matrix res1 = future1.get_value();
		Matrix res2 = future2.get_value();

		//combine mat1 and mat2 if necessary
	}

private:

	double operate(Matrix& matrix)
	{
		Matrix result;
		//some calculation on matrix

		return result;
	}
};

