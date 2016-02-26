#include "Bullet.h"

Bullet::Bullet(Vector3 pos, Vector3 size, Vector3 dir, int speed)
:speed(speed), position(pos), size(size), dir(dir)
{
    bulletObj = new Object(position, size);
}