#include "Hitbox.h"

Hitbox::Hitbox(Vector3 pos, Vector3 size)
{
	this->pos = pos;
	this->size = size;

	maxPt.x = pos.x + size.x / 2;
	maxPt.y = pos.y + size.y / 2;
	maxPt.z = pos.z + size.z / 2;
	minPt.x = pos.x - size.x / 2;
	minPt.y = pos.y - size.y / 2;
	minPt.z = pos.z - size.z / 2;
}

Hitbox::~Hitbox()
{
}

bool Hitbox::isTouching(Vector3 camPos)
{
	if ((camPos.x >= minPt.x) && (camPos.x <= maxPt.x) &&
		(camPos.y >= minPt.y) && (camPos.y <= maxPt.y) &&
		(camPos.z >= minPt.z) && (camPos.z <= maxPt.z)	)
	{
		return true;
	}
	else
	{
		return false;
	}
}