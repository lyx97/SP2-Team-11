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

NPC_CLASS::NPC_CLASS(string name, float health, Vector3 position, Vector3 size)
	:name(name), health(health), position(position), size(size)
{
	object = new Object(position, size);
}

void NPC_CLASS::setPos(Vector3 &pos){
	position = pos;
	*object = Object(position, size);
}