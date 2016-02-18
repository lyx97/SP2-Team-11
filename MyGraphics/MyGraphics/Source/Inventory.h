#pragma once
#include "Object.h"
#include "Singleton.h"

class Inventory
{
public:
	Inventory();
	virtual ~Inventory();

	static void addObject(Object* object);
};
