/******************************************************************************/
/*!
\file	Inventory.cpp
\author Lee Yu Xian
\par	email: 155118A\@mymail.nyp.edu.sg
\brief
Class to define the inventory system
*/
/******************************************************************************/
#include "Inventory.h"

/******************************************************************************/
/*!
\brief	Inventory default constructor
*/
/******************************************************************************/
Inventory::Inventory()
{
}

/******************************************************************************/
/*!
\brief	Inventory destructor
*/
/******************************************************************************/
Inventory::~Inventory()
{
}
/******************************************************************************/
/*!
\brief	Pass the objects to a map

\param	object
Object* value to be passed into map
*/
/******************************************************************************/
void Inventory::addObject(Object* object)
{
	Singleton::getInstance()->objectCount[object]++;
}
