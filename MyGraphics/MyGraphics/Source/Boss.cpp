#include "Boss.h"

Boss::Boss(string name, float health, Vector3 position, Vector3 size)
:name(name), health(health), position(position), size(size)
{
    object = new Object(position, size);
    sword = new Weapon(10);
    gun = new Weapon(5);
}

void Boss::setPos(Vector3 &pos){
    position = pos;
    *object = Object(position, size);
}
