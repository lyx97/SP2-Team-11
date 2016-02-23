#pragma once
#include "Hitbox.h"
#include <map>

using std::map;

class Object
{
public:
	Object(Vector3 pos = Vector3(0, 0, 0), Vector3 scale = Vector3(1, 1, 1));
	~Object();

	Hitbox hitbox;

	static map<Object*, int> objectMap;

	void setHealth(int health);
	void setGod();
	void receiveDmg(int dmg);

	int getHealth();

	bool isDead();

	Vector3 size;
	Vector3 pos;

private:
	int health;
	bool isGod;
};

