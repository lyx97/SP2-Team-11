#pragma once
#include "Object.h"
#include <map>

using std::map;
class Inventory
{
public:
	Inventory();
	virtual ~Inventory();

	void addObject(Object* add, int amount);
	int getAmount();
private:
	map<Object*, int> objectCount;
};
