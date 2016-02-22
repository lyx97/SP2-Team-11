#pragma once
#include "Hitbox.h"
#include <map>

using std::map;

class Object
{
public:
	Object(Vector3 pos = Vector3(0, 0, 0), Vector3 scale = Vector3(1, 1, 1));
	~Object();

	Hitbox hitbox;

	static map<Object*, int> objectMap;

	Vector3 size;
	Vector3 pos;
};

