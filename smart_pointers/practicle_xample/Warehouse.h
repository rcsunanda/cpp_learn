#pragma once

#include <map>
#include <memory>
#include <iostream>
#include <cassert>
#include "make_unique.h"


//-------------------------------------------------------------------------------------------------
struct Item
{
	Item(int field1, int field2):
		m_field1(field1), m_field2(field2)
	{}
	
	int m_field1;
	int m_field2;
};



//-------------------------------------------------------------------------------------------------
class ItemFactory
{
public:

	std::unique_ptr<Item> createItem(int field1, int field2)	//Q1 - return unique_ptr to indicate transfer of ownership to caller
	{
		return std::make_unique<Item>(field1, field2);
	}
};



//-------------------------------------------------------------------------------------------------
class Warehouse
{
public:

	void insertItem(std::unique_ptr<Item> item)	// Q3 - take unique_ptr to indicate that Warehouse takes ownership of Items
												//		- related to Q2 - is this too restrictive? (eg: what if caller has a shared_ptr?)
	{
		auto res = m_map1.insert(std::make_pair(item->m_field1, std::move(item)));
		assert(res.second == true);
	}

	Item& getItem(int key)	// Q4 - return reference to indicate you are referring to an object in the Warehouse and there is no transfer of ownership involved (similar to STL container getters)
							//		- is it ok to return raw pointer (Item*), eg: for nullability
							// Ans - this is only fine because we know that lifetime of Warehouse is a strict superset of lifetimes of Items it contains
	{
		auto iter = m_map1.find(key);
		assert(iter != std::end(m_map1));
		return *iter->second;
	}

	const Item& getItem(int key) const
	{
		// Same
	}

	std::unique_ptr<Item> popItem(int key)	// Q4 - return unique_ptr to indicate that Warehouse gives up ownership of the Item to the caller
	{
		auto iter = m_map1.find(key);
		assert(iter != std::end(m_map1));

		auto ret = std::move(iter->second);	// Type is std::unique_ptr<Item>

		m_map1.erase(iter);
		return ret;
	}

	void print()
	{
		std::cout << "Warehouse map key list: ";
		for (auto& entry: m_map1)
			std::cout << entry.first << ", ";
		std::cout << std::endl;
	}

private:

	std::map<int, std::unique_ptr<Item>> m_map1;	// Q2 - unique_ptr, as this is the only place Items are kept
													//		this is an important application-wide design decision and must be made carefully
													//		if we later have to store Items somewhere else too, we will have to make this shared_ptr and change the class API to reflect that
};



//-------------------------------------------------------------------------------------------------
class BigWarehouse
{
public:

	void insertItem(std::unique_ptr<Item> item)	// Q6 - take unique_ptr to indicate that BigWarehouse takes ownership of Items
	{
		// Q7 - convert to shared, and store in two maps

		std::shared_ptr<Item> sp1(item.release());
		auto sp2 = sp1;

		auto res1 = m_map1.insert(std::make_pair(item->m_field1, std::move(item)));
		auto res2 = m_map2.insert(std::make_pair(item->m_field2, std::move(item)));
		
		assert(res1.second == true);
		assert(res2.second == true);
	}

	Item& getItemByeKey1(int key1)	// Q7 - Similar to Warehouse
									//		- We can return a shared_ptr, but is that really necessary?
	{
		auto iter = m_map1.find(key1);
		assert(iter != std::end(m_map1));
		return *iter->second;
	}

	const Item& getItemByeKey1(int key1) const
	{
		// Same
	}

	// Similarly, getItemByeKey2()

	std::unique_ptr<Item> popItemByKey1(int key1)	// Q8 - return unique_ptr to indicate that BigWarehouse gives up ownership of the Item to the caller
	{
		auto iter1 = m_map1.find(key1);	// Assume element exists
		assert(iter1 != std::end(m_map1));

		auto sp1 = std::move(iter1->second);	// Type is std::shared_ptr<Item>, the move avoids an unnecessary shared_ptr copy (atomic increment and decrement)
		m_map1.erase(iter1);

		int key2 = sp1->m_field2;
		m_map2.erase(key2);	// Second shared_ptr is destroyed here

		assert(sp1.use_count() == 1);	// To verify that we now have only one shared_ptr to the Item
		std::unique_ptr<Item> ret(sp1.get());
		sp1.reset();
	}

	// Similarly, popItemByKey2()

	void print()
	{
		std::cout << "Warehouse map key list: ";
		for (auto& entry : m_map1)
			std::cout << entry.first << ", ";
		std::cout << std::endl;
	}

private:

	std::map<int, std::shared_ptr<Item>> m_map1;	// Q5 - shared_ptr as two maps keep pointers to Items
	std::map<int, std::shared_ptr<Item>> m_map2;	//		- if shared_ptr is too expensive, we can have one map with unique_ptrs (the map that owns Items), and the second map with raw pointers
};