/******************************************************************************/
/*!
\file	Inventory.h
\author Lee Yu Xian
\par	email: 155118A\@mymail.nyp.edu.sg
\brief
Class to define the inventory class
*/
/******************************************************************************/
#pragma once
#include "Object.h"
#include "Singleton.h"
/******************************************************************************/
/*!
Class Object:
\brief	Defines the inventory and its methods
*/
/******************************************************************************/
class Inventory
{
public:
	Inventory();
	~Inventory();

	static void addObject(Object* object);
};
