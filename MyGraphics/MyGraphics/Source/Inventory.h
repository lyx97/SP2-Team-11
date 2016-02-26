#pragma once
#include "Object.h"
#include "Singleton.h"

class Inventory
{
public:
	Inventory();
	~Inventory();

	static void addObject(Object* object);
};
