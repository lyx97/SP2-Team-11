#pragma once
#include "Vector3.h"

class Object
{
public:
	Object();
	~Object();

	void setHealth(int health);
	void setPos(Vector3 pos);
	bool isDead();

private:
	int health;
	Vector3 pos;

};

