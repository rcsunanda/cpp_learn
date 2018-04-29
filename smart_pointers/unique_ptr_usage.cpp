//g++ --std=c++11 -ggdb unique_ptr_usage.cpp -o unique_ptr_usage
//g++ --std=c++11 -ggdb -fsanitize=address unique_ptr_usage.cpp -o unique_ptr_usage

#include <memory>	//smart pointers
#include <string>
#include <cstdio>

#include "make_unique.h"
#include <cassert>


//*************************************************************************************************
class Animal
{
public:
	Animal(std::string name, int ID) :
		m_name(name), m_ID(ID)
	{}

	~Animal()
	{
		print("destructor");
	}

	Animal(const Animal&);

	void print(const char* prefix = "")
	{
		printf("Animal::print; prefix=%s; m_ID=%d; m_name=%s\n", prefix, m_ID, m_name.c_str());
	}

	std::string m_name;
	int m_ID;
};


//*************************************************************************************************
class AnimalFactory
{
public:
	static std::unique_ptr<Animal> createAnimal(std::string name, int ID)
	{
		// Implicit move operation into the variable that stores the result (ownership is transeferred to the caller: ideal for a factory)
		return std::make_unique<Animal>(name, ID);
	}
};


//*************************************************************************************************
void func_by_value_and_own(std::unique_ptr<Animal> ptr)
{
	ptr->m_ID += 100;
	//ptr is destroyed here; therefore caller must transfer ownership (call with std::move())
}

std::unique_ptr<Animal> func_by_value_own_and_yield(std::unique_ptr<Animal> ptr)
{
	ptr->m_ID += 100;
	return ptr;	//ptr is passed back to the caller via copy elision
}

void func_by_reference(const std::unique_ptr<Animal>& ptr)
{
	ptr->m_ID += 100;
	//since ptr is a reference, nothing happens here
}

//legacy code that takes raw pointers

void func_raw(Animal* ptr)
{
	ptr->m_ID += 100;
	// no destruction happens; therefore caller must retain ownership (via get())
}

void func_raw_own(Animal* ptr)
{
	ptr->m_ID += 100;
	delete ptr; //destroyed: caller must pass ownership of underlying pointer (via release())
}


//*************************************************************************************************
class ClassWithUniquePtr
{
public:
	ClassWithUniquePtr() {} //underlying pointer in m_animalPtr is null

	ClassWithUniquePtr(std::string animalName, int animalID) :
		m_animalPtr(std::make_unique<Animal>(animalName, animalID))
	{}

	void printAnimal()
	{
		assert(m_animalPtr);
		m_animalPtr->print("Printed from ClassWithUniquePtr::printAnimal");
	}

	//returning the Animal member (http://stackoverflow.com/questions/29498976/how-to-return-a-smart-pointer-to-a-member-variable)

	std::unique_ptr<Animal> getAnimalPtrWithOwnership()	//ownership is transferred to the caller
		//this is usually not good, as a private member shouldn't need to give up ownership this way
	{
		return std::move(m_animalPtr);	//explicit move is needed
	}

	Animal* getAnimalRawPtr() {	return m_animalPtr.get(); } //if nullability is needed

	Animal& getAnimalReference()
	{
		assert(m_animalPtr);
		return *m_animalPtr;
	}

private:
	std::unique_ptr<Animal> m_animalPtr;
};


//*************************************************************************************************
int main()
{
	// --------------------------------
	//creating unique_ptr and simple usage

	std::unique_ptr<Animal> ptr1(new Animal("A", 1));
	ptr1->print();

	auto ptr2 = std::make_unique<Animal>("B", 2);	//prefer to use make_unique (especially in function arguments: see https://herbsutter.com/gotw/_102/)
		//arguments to make unique are passed to the constructor of the object
	

	// --------------------------------
	//copying/moving unique_ptr

	//std::unique_ptr<Animal> ptr3 = ptr1; //error: unique_ptr cannot be copy constructed

	std::unique_ptr<Animal> ptr4 = std::move(ptr1); //allowed: ptr1 is now empty and not usable
	ptr4->print();
	//ptr1->print();	//UB (segfault)


	// --------------------------------
	//returning a unique_ptr from a function (measure overhead)

	auto ptr5 = AnimalFactory::createAnimal("C", 5);
	ptr5->print();

	//see also the get methods in class ClassWithUniquePtr

	// --------------------------------
	//passing unique_ptr to functions

	auto ptr6 = std::make_unique<Animal>("D", 6);
	auto ptr7 = std::make_unique<Animal>("E", 5);
	auto ptr8 = std::make_unique<Animal>("F", 8);
	auto ptr9 = std::make_unique<Animal>("G", 9);
	auto ptr10 = std::make_unique<Animal>("H", 10);

	//func_by_value(ptr6); //error: cannot pass by value; copy constructor is deleted

	func_by_value_and_own(std::move(ptr6));	//ownership is transferred (cannot use ptr6 after this)
	
	auto retPtr = func_by_value_own_and_yield(std::move(ptr7));	//ownership is transferred temporarily and returned to caller (The C++ Programming Language pg 989), preferred over func_by_reference

	func_by_reference(ptr8);	//ownership is retained

	func_raw(ptr9.get());	//ownership is retained (risk: we might call release() [unlikely], and the object will never be destroyed)

	func_raw_own(ptr10.release());	//ownership is transferred (risk: we might call get() [likely], and the object will be destroyed twice)


	// --------------------------------
	//unique_ptr member in class

	ClassWithUniquePtr obj1("obj1", 200);
	obj1.printAnimal();
	Animal& underlyingAnimal = obj1.getAnimalReference();	//LHS must be a reference (Animal&), otherwise, a new Animal is created on the stack via copy construction

	ClassWithUniquePtr obj2;
	obj2.getAnimalReference();	//will fail assertion

	///////http://stackoverflow.com/questions/8114276/how-do-i-pass-a-unique-ptr-argument-to-a-constructor-or-a-function
	//////https://herbsutter.com/2013/06/05/gotw-91-solution-smart-pointer-parameters/

	return 0;
}