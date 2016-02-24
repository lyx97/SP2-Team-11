#include "Weapon.h"


Weapon::Weapon(int damage) :
damage(damage)
{
}

Weapon::~Weapon()
{
}

void Weapon::setDamage(int damage)
{
	this->damage = damage;
}

int Weapon::getDamage()
{
	return damage;
}