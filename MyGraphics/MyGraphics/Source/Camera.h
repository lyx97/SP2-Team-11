#ifndef CAMERA_H
#define CAMERA_H
#include "Vector3.h"
#include "Object.h"

class Camera
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	Vector3 view;
	Vector3 right;

	Vector3 boundCheck;

	bool flying = false;
	bool jumping = false;
	bool togJump = false;
	float delay = 4.f;
	float jumpDelay = 9.f;
	double mousex, mousey;

	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt, vector<Object*> objectVec);
	virtual void Reset();
	void EnterShip(Vector3& planePos, double dt);
	bool collision(Vector3& camPos);
};

bool planeHitbox(Vector3& camPos);
bool chopperHitbox(Vector3& camPos);
#endif