#include "Object.h"

vector<Object*> Object::objectVec;

Object::Object(Vector3 pos, Vector3 size):
hitbox(Hitbox(pos, size))
{
	this->pos = pos;
	this->size = size;
	Object::objectVec.push_back(this);
}

Object::~Object()
{
}

