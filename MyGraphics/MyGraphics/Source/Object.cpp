#include "Object.h"

vector<Object*> Object::objectVec;

Object::Object(Vector3 pos, Vector3 scale):
hitbox(pos, scale)
{
	objectVec.push_back(this);
}

Object::~Object()
{
}

void Object::setPos(float posX, float posY, float posZ)
{
	this->pos.x = posX;
	this->pos.y = posY;
	this->pos.z = posZ;
}

void Object::setSize(float scaleX, float scaleY, float scaleZ)
{
	this->size.x = scaleX;
	this->size.y = scaleY;
	this->size.z = scaleZ;
}

Vector3 Object::getPos()
{
	return pos;
}

Vector3 Object::getSize()
{
	return size;
}

