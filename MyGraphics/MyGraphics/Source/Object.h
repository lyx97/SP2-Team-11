#pragma once
#include "Vector3.h"
#include "Hitbox.h"
#include <vector>

using std::vector;

class Object
{
public:
	Object(Vector3 pos = Vector3(0, 0, 0), Vector3 scale = Vector3(1, 1, 1));
	~Object();

	void setPos(float posX, float posY, float posZ);
	void setSize(float scaleX, float scaleY, float scaleZ);

	Vector3 getPos();
	Vector3 getSize();

	Hitbox hitbox;

	static vector<Object*> objectVec;

private:
	Vector3 size;
	Vector3 pos;
};

