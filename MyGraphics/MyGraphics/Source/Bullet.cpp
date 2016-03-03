/******************************************************************************/
/*!
\file	Object.cpp
\author Lee Yu Xian
\author Sri Sham
\par	email: 155118A\@mymail.nyp.edu.sg
\par	email: 15#####\@mymail.nyp.edu.sg
\brief
Class to define the Bullet class
*/
/******************************************************************************/
#include "Bullet.h"

vector<Bullet*> Bullet::bulletVec;
vector<Bullet*> Bullet::bossBulletVec;

/******************************************************************************/
/*!
\brief	Bullet default constructor

\param	pos
defines position of bullet when initialised

\param	size
defines size of bullet when initialised

\param	dir
defines direction of bullet when initialised

\param speed
defines speed of bullet when initialised

\param player

*/
/******************************************************************************/
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