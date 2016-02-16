#pragma once
#include "Object.h"
#include "Vector3.h"

class Hitbox : public Object
{
public:
	Hitbox();
	~Hitbox();

	void setPos(float posx, float posy, float posz);
	void setSize(unsigned int scalex, unsigned int scaley, unsigned int scalez);

private:
	Vector3 pos;
	Vector3 size;
};

