//g++ --std=c++11 -ggdb raw_ptr_usage.cpp -o raw_ptr_usage
//g++ --std=c++11 -ggdb -fsanitize=address raw_ptr_usage.cpp -o raw_ptr_usage

#include <string>
#include <cstdio>


class Animal
{
public:
	Animal(std::string name, int ID):
		m_name(name), m_ID(ID)
	{}

	~Animal()
	{
		printf("Animal::~Animal; m_ID=%d\n", m_ID);
	}

	std::string m_name;
	int m_ID;
};


//*************************************************************************************************
int main()
{
	Animal* ptr = nullptr;
	
	delete ptr;	//deleting a nullptr

	ptr = new Animal("tommy", 1);

	delete ptr;

	//access the pointer after delete: valid
	printf("ptr-after-delete=%p\n", ptr);

	//access object pointed to by the pointer after delete: UB
	//int ID = ptr->m_ID;	//AddressSanitizer: heap-use-after-free
	//Animal x = *ptr;		//(AddressSanitizer: SEGV on unknown address)

	//check value here fater delete

	return 0;
}