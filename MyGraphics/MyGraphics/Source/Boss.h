#ifndef BOSS_H
#define BOSS_H

#include <string>
#include <Vector3.h>
#include "Object.h"
#include "Weapon.h"

using std::string;

class Boss
{
public:

    virtual ~Boss()
    {
    }

    Boss(string name = "BOSS", int health = 100, Vector3 position = Vector3(0, 0, 0), Vector3 size = Vector3(1,1,1));

    void setPos(Vector3 &pos);

    string name;
    int health;
    Vector3 position;
    Vector3 size;
    Object* object;
    Weapon* sword;
    Weapon* gun;
};

#endif /*BOSS_H*/