/******************************************************************************/
/*!
\file	Object.cpp
\author Lee Yu Xian
\par	email: 155118A\@mymail.nyp.edu.sg
\brief
Class to define the object class
*/
/******************************************************************************/
#include "Object.h"

map<Object*, int> Object::objectMap;
/******************************************************************************/
/*!
\brief	Object default constructor

\param	pos
defines position of object when initialised and set the position of hitbox at that position

\param	size
defines size of object when initialisedand set the size of hitbox

\param	godMode
set if object is unkillable when initialised
*/
/******************************************************************************/
Object::Object(Vector3 pos, Vector3 size, bool godMode) :
hitbox(Hitbox(pos, size)),
health(100)
{
	this->pos = pos;
	this->size = size;
	Object::objectMap[this]++;
}
/******************************************************************************/
/*!
\brief	Object destructor
*/
/******************************************************************************/
Object::~Object()
{
}
/******************************************************************************/
/*!
\brief	Set the health of the object

\param	health
health value to be passed into the object
*/
/******************************************************************************/
void Object::setHealth(int health)
{
	this->health = health;
}
/******************************************************************************/
/*!
\brief	Set the position of the object

\param	position
position value to be passed into the object
*/
/******************************************************************************/
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
/******************************************************************************/
/*!
\brief	Allow Object to receive damage

\param	damage
damage value to be passed into the object to decrease health
*/
/******************************************************************************/
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
/******************************************************************************/
/*!
\brief	Get the health of the object

\return	health
health value of the object
*/
/******************************************************************************/
int Object::getHealth()
{
	return health;
}
/******************************************************************************/
/*!
\brief	check of object is dead

\return	TRUE if object is dead
*/
/******************************************************************************/
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