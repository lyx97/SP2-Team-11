/******************************************************************************/
/*!
\file	Boss.h
\author Sri Sham Haran
\par
\brief
Class to define the Boss class
*/
/******************************************************************************/

#ifndef BOSS_H
#define BOSS_H

#include <string>
#include <vector>
#include <Vector3.h>
#include "Object.h"
#include "Weapon.h"
#include "Bullet.h"

using std::string;
using std::vector;

/******************************************************************************/
/*!
Class Boss:
\brief	Defines the boss and its methods
*/
/******************************************************************************/
class Boss
{
public:

    virtual ~Boss()
    {
    }

    Boss(string name = "BOSS", float health = 100, Vector3 position = Vector3(0, 0, 0), Vector3 size = Vector3(1,1,1));

    void setPos(Vector3 &pos);

    string name;
    float health;
    Vector3 position;
    Vector3 size;
    Object* object;
    Weapon* sword;
    Weapon* gun;
};

#endif /*BOSS_H*/