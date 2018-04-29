#pragma once

#include <mutex>
#include <iostream>


class ThreadSafeClass
{
public:
	//********************************************************************************************
	//Case 1: std::mutex

	void print_1()	//Its' not safe to use mutex.lock() and unlock() directly --> might forget to unlock/ unlocking must be done on all paths (eg: exceptions) --> see print_2()
	{
		m_mutex.lock();
		std::cout << "print_1(): This part runs concurrently but mutually exclusively, thread id: " << std::this_thread::get_id() << "m_data: " << m_data << '\n';
		m_mutex.unlock();
	}


	//********************************************************************************************
	//Case 2: std::mutex with std::lock_guard

	void print_2()	//If every function is like this, this appears to becomes a thread-safe class. But if a pointer or reference to m_data is given outside, the protection breaks
					//This can happen by returning pointers/references or letting user-defined functions passed as arguments to member functions access them
					//Therefore, the class/code must be structred to prevent such cases
	{
		std::lock_guard<std::mutex> gaurd(m_mutex);	//m_mutex is implicitly locked in gaurd's constructor (if you don't want to lock, provide a second argument std::adopt_lock)
		std::cout << "print_1(): This part runs concurrently but mutually exclusively, thread id: " << std::this_thread::get_id() << "m_data: " << m_data << '\n';
	}	//when 'gaurd' is destroyed, m_mutex is implicitly unlocked


	//********************************************************************************************
	//Case 3: Race conditions inherent in interfaces
	//Protecting each member function and structuring them to leak out pointers/references to private members is not enough
	//Interfaces can have inherant race conditions
	//eg: If we wrap std::stack<T> along with a mutex and create a ThreadSafeStack<T>, there're several interface problems
		// - cannot rely upon empty() and size(); by the time you move to the next line of a thread, the results taken from these calls can have changed

	//this is how we would normally pop values from a stack and use them (need to check empty() first before taking top, otherwise undefined behaviour)
	/*
	ThreadSafeStack<int> stack;
	if (!stack.empty())
	{
		int const topValue = stack.top();	//probelm 1: by the time we're here, the stack may be empty (from actions of another thread) --> undefined behaviour
		stack.pop();
		//process(topValue)
	}
	*/
	//problem 2: Two threads might simultaneously execute top() and see the same value.
		//Then they both will execute pop() and 2 elements are removed; one of them is discarded without being seen --> more dangerous than problem 1 (we may never notice)

	//The above problem still exists for lock-free data structures, it is an interface problem

	//Possible solution: combine calls to top() and pop() under protection of one mutex in the function pop()
	//But there's a problem here too:
		//When returning from the new pop(), the topValue must be returned to the caller and the copy constructor of type T is invoked (isn't this the move c'tor in C++11?? - check)
		//The copy c'tor might throw an exception (eg: if T was a large type such as vector<int>, and if memory allocation fails, copy c'tor throws an std::bad_alloc exception)
		//Then topValue is lost (it was removed from the ThreadSafeStack and it could not be returned)
		//This is infact one of the reasons why std::stack<T> has top() and pop() separately

	//Option 1: pass in a reference; pop() signature would be --> void pop(T& value) { ... }
	
	//Option 2: require that type T has no-throw copy constructor or move constructor --> this can limit the usability of the ThreadSafeStack
	
	//Option 3: return a pointer to the popped element; pop() signature would be --> std::shared_ptr<T> pop() { ... }
		//Pro: pointers can be freely copied without throwing an exception
		//Con: requires a means of managing the memory allocated to the object (for simple types like integers, the overhead of such memory management is much larger than returning by value)
			//To avoid this con: use std::shared_ptr<T>

	//Option 4: proved options 1 and one of 2 or 3


	//********************************************************************************************
	//Case 4: using std::lock() to avoid deadlocks

	void swap_1(ThreadSafeClass& lhs, ThreadSafeClass& rhs)
	{
		if (&lhs == &rhs)	//ensure they are different instances; locking the same mutex twice in one thread leads to undefined behaviour
			return;

		std::lock(lhs.m_mutex, rhs.m_mutex);	//lock the 2 locks while ensuring deadlock-free conditions (what's the technique/ algorithm?)
		
		std::lock_guard lhsGaurd(lhs.m_mutex, std::adopt_lock);	//std::adopt_lock --> gaurd constructor assumes calling thread already owns and has already locked the mutex
		std::lock_guard rhsGaurd(rhs.m_mutex, std::adopt_lock);

		swap(lhs.m_data, rhs.m_data);	//actual processing

	}	//mutexes are released implicitly when gaurds are destroyed


	//********************************************************************************************
	//Case 5: using std::unique_lock

	void swap_2(ThreadSafeClass& lhs, ThreadSafeClass& rhs)	//same functionality as above
	{
		if (&lhs == &rhs)	//ensure they are different instances; locking the same mutex twice in one thread leads to undefined behaviour
			return;

		std::unique_lock<std::mutex> lhsLock(lhs.m_mutex, std::defer_lock);	//std::defer_lock --> mutex should remain unlocked on construction
		std::unique_lock<std::mutex> rhsLock(rhs.m_mutex, std::defer_lock);

		std::lock(lhs.m_mutex, rhs.m_mutex);	//lock the 2 locks while ensuring deadlock-free conditions

		swap(lhs.m_data, rhs.m_data);	//actual processing

	}	//mutexes are released implicitly when unique_locks are destroyed

	//Pros/cons of unique_lock over lock_guard
		//unique_lock takes a bit more space and slighly slower (may be neglegible)
		//std::unique_lock instance doesn’t always own the mutex --> allows transwer of ownership
		//unique_lock can call lock(), unlock(), try_lock(), and other timed locking functions
			//so it can be just as flexible as the underlying mutex while gauranteeing unlock upon destructop 

	//********************************************************************************************
	//Case 6: std::unique_lock --> transfering ownersip of underlying mutex via the move constructor

	std::unique_lock<std::mutex> getLock()
	{
		std::unique_lock<std::mutex> lock(m_mutex);	//mutex is locked here (it's not unlocked within this function --> see below processData();

		++m_data;	//data is modified while holding the lock

		return lock;	//theoratically, compiler is free to do return value optimization (no copy happens); or it can use the move constructor
						//the book says this will use the move constructor --> ownership of the underlying mutex is transfered to the caller
	}

	void processData()
	{
		std::unique_lock<std::mutex> myLock(getLock());	//ownership of the underlying mutex is transfered to myLock
		m_data = 2 * m_data;
	}	//lock that was acquired in first line of getLock() is released here

	//The ownership transfer is useful in cases where you do thread-safe access to some data via a gateway class (the unique_lock is then a member of this class)
	//When a gateway object is created, lock is acquired, when its destroyed, lock is released. In between, you can safely access data via the gateway object
	//If we need to write a move constructor for the gateway class (so that it can be returned from a function), the unique_lock data member must also be movable
	//This is made possible by the fact that std::unique_lock is movable and it transfers the ownership of the underlying mutex


private:
	int m_data;
	std::mutex m_mutex;

	void swap(int& lhs, int& rhs)
	{
		int temp = lhs;
		lhs = rhs;
		rhs = temp;
	}
};