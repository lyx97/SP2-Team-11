#include "Hitbox.h"


Hitbox::Hitbox()
{
	pos = (0, 0, 0);
	size = (1, 1, 1);
}

Hitbox::~Hitbox()
{
}

void Hitbox::setPos(float posx, float posy, float posz)
{
	pos = (posx, posy, posz);
}

void Hitbox::setSize(unsigned int scalex, unsigned int scaley, unsigned int scalez)
{
	size = (scalex, scaley, scalez);
}
