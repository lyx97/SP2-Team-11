#pragma once
#include "Vector3.h"
#include "Object.h"
#include "Singleton.h"
#include <vector>

using std::vector;

class Bullet : public Object
{
public:
	Bullet(Vector3 pos = Vector3(0, 0, 0), Vector3 size = Vector3(1, 1, 1), Vector3 dir = Vector3(0, 0, 0), int speed = 1);
	~Bullet();

	void setPos(Vector3 pos);
	void setSize(Vector3 size);

	static vector<Bullet*> bulletVec;

	Vector3 dir;


	int speed;
};

