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

		using Task_type = double(Matrix& matrix); // type of task function (operate)

		std::packaged_task<Task_type> task1{ &Calculator::operate }; // package the task (i.e. operate function)
		std::packaged_task<Task_type> task2{ &Calculator::operate };

		future<double> future1{ task1.get_future() }; // get associated future
		future<double> future2{ task2.get_future() };

		std::thread thread1(move(task1), this, mat1);
		std::thread thread2(move(task2), this, mat2);

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

