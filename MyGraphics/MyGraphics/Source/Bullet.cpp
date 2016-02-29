#include "Bullet.h"

vector<Bullet*> Bullet::bulletVec;

Bullet::Bullet(Vector3 posit, Vector3 size, Vector3 dir, int speed) :
dir(dir),
speed(speed),
Object(pos, size)
{
	Bullet::bulletVec.push_back(this);
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