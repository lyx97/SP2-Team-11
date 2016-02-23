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

void Object::setHealth(int health)
{
	this->health = health;
}

void Object::receiveDmg(int dmg)
{
	if (!isGod)
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

void Object::setGod()
{
	isGod = true;
}