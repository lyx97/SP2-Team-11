/******************************************************************************/
/*!
\file	Boss.cpp
\author Sri Sham
\par	email: 155129R\@mymail.nyp.edu.sg
\brief
Class to define the Boss class
*/
/******************************************************************************/
#include "Boss.h"

/******************************************************************************/
/*!
\brief	Boss default constructor

\param	name
defines name of boss when initialized

\param	health
defines size of boss when initialized

\param	position
defines position of boss when initialised

\param size
defines size of boss when initialised

*/
/******************************************************************************/
Boss::Boss(string name, float health, Vector3 position, Vector3 size)
:name(name), health(health), position(position), size(size)
{
    object = new Object(position, size);
    sword = new Weapon(10);
    gun = new Weapon(5);
}

/******************************************************************************/
/*!
\brief	Method to update/set position of boss

\param	pos
defines position of boss when initialised

*/
/******************************************************************************/
void Boss::setPos(Vector3 &pos){
    position = pos;
    *object = Object(position, size);
}
