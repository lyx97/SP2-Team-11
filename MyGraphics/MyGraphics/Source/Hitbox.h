#pragma once
#include "Vector3.h"

class Hitbox
{
public:
	Hitbox(Vector3 pos = Vector3(0, 0, 0), Vector3 scale = Vector3(1, 1, 1));
	~Hitbox();

	bool isTouching(Vector3 camPos);

	Vector3 maxPt, minPt;
	Vector3 size;
	Vector3 pos;
private:
};

