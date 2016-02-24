#include "Object.h"

map<Object*, int> Object::objectMap;

Object::Object(Vector3 pos, Vector3 size, bool godMode) :
hitbox(Hitbox(pos, size)),
health(100)
{
	this->pos = pos;
	this->size = size;
	Object::objectMap[this]++;
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

void Object::receiveDmg(int dmg)
{
	if (godMode)
	{
	}
	else
	{
		health -= dmg;
	}
}

int Object::getHealth()
{
	return health;
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