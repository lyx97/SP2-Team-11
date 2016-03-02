#pragma once

class Weapon
{
public:
	Weapon(int damage = 0);
	~Weapon();

	int getDamage();

private:
	int damage;
};

