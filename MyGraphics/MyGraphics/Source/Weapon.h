#pragma once
#include "Object.h"

class Weapon : public Object
{
public:
	Weapon(int damage = 0);
	~Weapon();

	void setDamage(int damage);
	
	int getDamage();

private:
	int damage;
};

