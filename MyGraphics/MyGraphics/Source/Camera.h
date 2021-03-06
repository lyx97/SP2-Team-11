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
    Vector3 shipPos = Vector3(0,0,0);
	Vector3 view;
	Vector3 right;

	Vector3 boundCheck;

	float delay = 4.f;
	float ySpeed = 0.f;
	float yaw;
	float pitch;

	double mousex, mousey;

	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
};

#endif