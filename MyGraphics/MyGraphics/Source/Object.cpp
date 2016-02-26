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
	hitbox.pos = pos;

	hitbox.maxPt.x = this->pos.x + size.x / 2;
	hitbox.maxPt.y = this->pos.y + size.y / 2;
	hitbox.maxPt.z = this->pos.z + size.z / 2;
	hitbox.minPt.x = this->pos.x - size.x / 2;
	hitbox.minPt.y = this->pos.y - size.y / 2;
	hitbox.minPt.z = this->pos.z - size.z / 2;
}

void Object::receiveDmg(int damage)
{
	if (godMode)
	{
	}
	else
	{
		health -= damage;
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