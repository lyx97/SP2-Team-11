#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"

class Camera3 : public Camera
{
public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Vector3 view;
	Vector3 right;

	Vector3 boundCheck;

	bool flying = false;
	bool jumping = false;
	bool togJump = false;
	float delay = 4.f;
	float jumpDelay = 9.f;

	double mousex, mousey;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
	void EnterShip(Vector3& planePos, double dt);

};

bool planeHitbox(Vector3& camPos);
bool chopperHitbox(Vector3& camPos);
#endif