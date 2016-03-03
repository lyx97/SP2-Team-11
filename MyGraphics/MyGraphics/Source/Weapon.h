/******************************************************************************/
/*!
\file	Weapon.h
\author Lee Yu Xian
\par	email: 155118A\@mymail.nyp.edu.sg
\brief
Struct to define the object class
*/
/******************************************************************************/
#pragma once
/******************************************************************************/
/*!
Class Weapon:
\brief	Defines a weapon and its methods
*/
/******************************************************************************/
class Weapon
{
public:
	Weapon(int damage = 0);
	~Weapon();

	int getDamage();

private:
	int damage;
};

