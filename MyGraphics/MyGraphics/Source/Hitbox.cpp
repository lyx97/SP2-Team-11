/******************************************************************************/
/*!
\file	Hitbox.cpp
\author Lee Yu Xian
\par	email: 155118A\@mymail.nyp.edu.sg
\brief
Class to define the hitbox class
*/
/******************************************************************************/
#include "Hitbox.h"
/******************************************************************************/
/*!
\brief	Hitbox default constructor

\param	pos
defines position of hitbox when initialised

\param	size
defines size of hitbox when initialised
*/
/******************************************************************************/
Hitbox::Hitbox(Vector3 pos, Vector3 size)
{
	this->pos = pos, this->size = size;

	maxPt.x = pos.x + size.x / 2;
	maxPt.y = pos.y + size.y / 2;
	maxPt.z = pos.z + size.z / 2;
	minPt.x = pos.x - size.x / 2;
	minPt.y = pos.y - size.y / 2;
	minPt.z = pos.z - size.z / 2;
}
/******************************************************************************/
/*!
\brief	Object destructor
*/
/******************************************************************************/
Hitbox::~Hitbox()
{
}
/******************************************************************************/
/*!
\brief	Check of hitbox is touching anything else

\return	TRUE if something is touching the hitbox
*/
/******************************************************************************/
bool Hitbox::isTouching(Vector3 camPos)
{
	if ((camPos.x >= minPt.x) && (camPos.x <= maxPt.x) &&
		(camPos.y >= minPt.y) && (camPos.y <= maxPt.y) &&
		(camPos.z >= minPt.z) && (camPos.z <= maxPt.z)	)
	{
		return true;
	}
	else
	{
		return false;
	}
}