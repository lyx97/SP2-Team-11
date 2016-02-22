#include "Object.h"

map<Object*, int> Object::objectMap;

Object::Object(Vector3 pos, Vector3 size):
hitbox(Hitbox(pos, size))
{
	this->pos = pos;
	this->size = size;
	Object::objectMap[this]++;
}

Object::~Object()
{
}

