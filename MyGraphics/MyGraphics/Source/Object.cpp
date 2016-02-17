#include "Object.h"

vector<Object*> Object::objectVec;

Object::Object(Vector3 pos, Vector3 scale):
hitbox(pos, scale)
{
	Object::objectVec.push_back(this);
}

Object::~Object()
{
}

void Object::setPos(float x, float y, float z)
{
	this->pos.x = x;
	this->pos.y = y;
	this->pos.z = z;
	hitbox.pos.x = x;
	hitbox.pos.y = y;
	hitbox.pos.z = z;
}

void Object::setSize(float scaleX, float scaleY, float scaleZ)
{
	this->size.x = scaleX;
	this->size.y = scaleY;
	this->size.z = scaleZ;
	hitbox.size.x = scaleX;
	hitbox.size.y = scaleY;
	hitbox.size.z = scaleZ;
}

