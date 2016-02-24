#ifndef BULLET_H
#define BULLET_H

#include <Vector3.h>
#include "Object.h"

class Bullet{
    Bullet();
    ~Bullet();

    int speed;
    Object* bulletObj;
    Vector3 position;
    Vector3 dir;

};

#endif /*BULLET_H*/