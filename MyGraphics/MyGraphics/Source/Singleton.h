#pragma once
#include "Vector3.h"
#include <map>
#include "Object.h"
#include "Inventory.h"

using std::map;

class Singleton
{
public:
	static Singleton* getInstance()
	{
		static Singleton instance;

		return &instance;
	}
	bool pause;
	bool buttonText;
	map<Object*, int> objectCount;

private:
	Singleton(){}
};

