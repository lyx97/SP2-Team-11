#ifndef BULLET_H
#define BULLET_H

#include <Vector3.h>
#include "Object.h"

class Bullet
{
public:
    Bullet(Vector3 pos, Vector3 size, Vector3 dir, int speed);
    ~Bullet();
    Object* bulletObj;
    Vector3 position;
    Vector3 dir;
    int speed;
    Vector3 size;
};

#endif /*BULLET_H*/