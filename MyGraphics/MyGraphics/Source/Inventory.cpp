#include "Inventory.h"


Inventory::Inventory()
{
}


Inventory::~Inventory()
{
}

void Inventory::addObject(Object* object)
{
	Singleton::getInstance()->objectCount[object]++;
}
