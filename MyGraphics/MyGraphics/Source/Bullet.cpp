#include "Bullet.h"

vector<Bullet*> Bullet::bulletVec;
vector<Bullet*> Bullet::bossBulletVec;


Bullet::Bullet(Vector3 pos, Vector3 size, Vector3 dir, int speed, bool player) :
dir(dir),
speed(speed),
position(pos),
Object(pos, size)
{
    if (player == true)
        Bullet::bulletVec.push_back(this);
    else{
        Bullet::bossBulletVec.push_back(this);
    }
}

Bullet::~Bullet()
{
}

void Bullet::setPos(Vector3 pos)
{
	this->pos = pos;
}

void Bullet::setSize(Vector3 size)
{
	this->size = size;
}