#include "Warehouse.h"

int main()
{
	Warehouse warehouse;
	ItemFactory factory;

	auto item1 = factory.createItem(1, 2);	// Type is std::unique_ptr<Item>
	auto item2 = factory.createItem(11, 22);

	warehouse.insertItem(std::move(item1));
	warehouse.insertItem(std::move(item2));

	warehouse.print();

	Item& item3 = warehouse.getItem(1);
	
	// Do something with item3, but no need to worry about ownership (as we got a reference, which means we don't own it)


	auto item4 = warehouse.popItem(1);	// Type is std::unique_ptr<Item>
	
	// Do something with item4, and it will be destroyed when the unique_ptr goes out of scope

	// Note: Exception safety
	// Since item1, item2, item4 are all smart pointers, we don't need to worry about potential memory leaks caused by any throwing functions called in this code

	return 0;
}