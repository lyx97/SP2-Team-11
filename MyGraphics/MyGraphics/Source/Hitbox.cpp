#include "Hitbox.h"


Hitbox::Hitbox(Vector3 pos, Vector3 size)
{
	this->size = size;

	this->pos = pos;

	maxPt.x = pos.x + size.x;
	maxPt.y = pos.y + size.y;
	maxPt.z = pos.z + size.z;

	minPt.x = -pos.x - size.x;
	minPt.y = -pos.y - size.y;
	minPt.z = -pos.z - size.z;
}

Hitbox::~Hitbox()
{
}

bool Hitbox::isTouching(Vector3 camPos)
{
	if ((camPos.x > minPt.x) && (camPos.x < maxPt.x) &&
		(camPos.y > minPt.y) && (camPos.y < maxPt.y) &&
		(camPos.z > minPt.z) && (camPos.z < maxPt.z))
	{
		return false;
	}
	else
	{
		return true;
	}
}