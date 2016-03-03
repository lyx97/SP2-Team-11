/******************************************************************************/
/*!
\file	Weapon.cpp
\author Lee Yu Xian
\par	email: 155118A\@mymail.nyp.edu.sg
\brief
Struct to define the weapon class
*/
/******************************************************************************/
#include "Weapon.h"

/******************************************************************************/
/*!
\brief	Weapon default constructor

\param damage
defines the damage of the weapon when initialised
*/
/******************************************************************************/
Weapon::Weapon(int damage) :
damage(damage)
{
}
/******************************************************************************/
/*!
\brief	Weapon destructor
*/
/******************************************************************************/
Weapon::~Weapon()
{
}
/******************************************************************************/
/*!
\brief	Get the damage of the weapon

\return	damage
damage value of the weapon
*/
/******************************************************************************/
int Weapon::getDamage()
{
	return damage;
}