#ifndef NPC_H
#define NPC_H

#include <string>
#include <vector>
#include <Vector3.h>
#include "Object.h"

using std::string;
using std::vector;

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

#endif /*BOSS_H*/