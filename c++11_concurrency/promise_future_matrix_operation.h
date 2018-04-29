#pragma once

#include <thread>

class Matrix;

//This class performs a matrix operation concurrently by executing it on two threads
//Results of operations are retrieved from the threads via a promise/future pair

class Calculator
{
public:

	void doConcurrentOperation(Matrix& matrix)
	{
		//split matrix into mat1 and mat2 (for data parallelism)

		std::promise<Matrix> promise1;	//create a promise
		std::future<Matrix> future1 = promise1.get_future();	//get associated future

		std::promise<Matrix> promise2;
		std::future<Matrix> future2 = promise2.get_future();

		std::thread t1(&Calculator::operate, this, mat1, promise1);
		std::thread t2(&Calculator::operate, this, mat2, promise2);

		Matrix res1 = future1.get_value();	
		Matrix res2 = future2.get_value();

		//combine mat1 and mat2 if necessary

		t1.join();
		t2.join();
	}

private:

	void operate(Matrix& matrix, std::promise<Matrix>& promise)
	{
		Matrix result;
		//some calculation on matrix

		promise.set_value(result);
	}
};

