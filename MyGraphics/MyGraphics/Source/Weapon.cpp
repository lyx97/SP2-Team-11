#include "Weapon.h"


Weapon::Weapon(int damage) :
damage(damage)
{
}

Weapon::~Weapon()
{
}

int Weapon::getDamage()
{
	return damage;
}