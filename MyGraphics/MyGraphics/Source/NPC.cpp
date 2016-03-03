/****************************************************************************/
/*!
\file NPC.cpp
\author Teo Jia Hao
\par email: kelvinteojiahao\@hotmail.com
\brief
Class to define an NPC
*/
/****************************************************************************/
#include "NPC.h"

/******************************************************************************/
/*!
\brief	NPC default constructor

\param	name
defines name of NPC when initialized

\param	health
defines size of NPC when initialized

\param	position
defines position of NPC when initialised

\param size
defines size of NPC when initialised

*/
/******************************************************************************/
NPC_CLASS::NPC_CLASS(string name, float health, Vector3 position, Vector3 size):
name(name), 
health(health), 
position(position), 
size(size)
{
	object = new Object(position, size);
}
/******************************************************************************/
/*!
\brief	Method to update/set position of NPC

\param	pos
defines position of NPC when initialised

*/
/******************************************************************************/
void NPC_CLASS::setPos(Vector3 &pos){
	position = pos;
	*object = Object(position, size);
}