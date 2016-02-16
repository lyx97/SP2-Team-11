#include "Object.h"


Object::Object()
{
	health = 100;
	pos = (0, 0, 0);
}

Object::~Object()
{
}

void Object::setHealth(int health)
{
	this->health = health;
}

void Object::setPos(Vector3 pos)
{
	this->pos = pos;
}

bool Object::isDead()
{
	if (health <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
