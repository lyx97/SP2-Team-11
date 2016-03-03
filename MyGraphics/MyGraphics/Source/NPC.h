/****************************************************************************/
/*!
\file NPC.h
\author Teo Jia Hao
\par email: kelvinteojiahao\@hotmail.com
\brief
Class to define an NPC
*/
/****************************************************************************/
#ifndef NPC_H
#define NPC_H

#include <string>
#include <vector>
#include <Vector3.h>
#include "Object.h"

using std::string;
using std::vector;

/****************************************************************************/
/*!
Class NPC_CLASS:
\brief Defines a NPC class and its methods
*/
/****************************************************************************/
class NPC_CLASS
{
public:

	virtual ~NPC_CLASS()
	{
	}

	NPC_CLASS(string name = "NPC", float health = 100, Vector3 position = Vector3(0, 0, 0), Vector3 size = Vector3(1, 1, 1));

	void setPos(Vector3 &pos);

	string name;
	float health;
	Vector3 position;
	Vector3 size;
	Object* object;
};

#endif /*NPC_H*/