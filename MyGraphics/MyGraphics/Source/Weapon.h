#pragma once

class Weapon
{
public:
	Weapon(int damage = 0);
	~Weapon();

	void setDamage(int damage);
	
	int getDamage();

private:
	int damage;
};

