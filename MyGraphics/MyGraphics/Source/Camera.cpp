#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"
#include <GLFW\glfw3.h>

#include <iostream>

using std::cout;
using std::endl;

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	view = (target - position).Normalized();
	right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = right.Cross(view).Normalized();
}
bool Camera::collision(Vector3& camPos)
{
	//sword distance
	if (sqrtf((0.f - camPos.x) * (0.f - camPos.x) + (0.f - camPos.z) * (0.f - camPos.z)) < 5) return false;

	//gun distance
	else if (sqrtf((-50.f - camPos.x) * (-50.f - camPos.x) + (0.f - camPos.z) * (0.f - camPos.z)) < 5) return false;

	else return true;
}
bool bound(Vector3& camPos)
{
	Vector3 maxPos(300, 0, 300);
	Vector3 minPos(-300, 0, -300);
	if ((maxPos.x > camPos.x && minPos.x < camPos.x) &&
		(maxPos.z > camPos.z && minPos.z < camPos.z)	)
	{
		return true;
	}
	else
	{
		return true;
	}
}

bool planeHitbox(Vector3& camPos)
{
	Vector3 maxPos(30, 0, 70);
	Vector3 minPos(-10, 0, 33);
	if ((camPos.x > minPos.x && camPos.x < maxPos.x) &&
		(camPos.z > minPos.z && camPos.z < maxPos.z))
	{
		return true;
	}
	else 
	{
		return false;
	}
}

bool chopperHitbox(Vector3& camPos)
{
	Vector3 maxPos(95, 0, 155);
	Vector3 minPos(85, 0, 145);
	if ((camPos.x > minPos.x && camPos.x < maxPos.x) &&
		(camPos.z > minPos.z && camPos.z < maxPos.z))
	{
		return true;
	}
	else 
	{
		return false;
	}
}

void Camera::EnterShip(Vector3& planePos, double dt)
{	
	static const float LOOKING_SPEED = 20.f;

	Application::MouseMove(mousex, mousey);
	float yaw = LOOKING_SPEED * dt * static_cast<float>((SCREEN_WIDTH / 2) - mousex);
	float pitch = LOOKING_SPEED * dt * static_cast<float>((SCREEN_HEIGHT / 2) - mousey);

	if (yaw != 0)
	{
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		position.x = target.x - view.x * 100.f;
		position.z = target.z - view.z * 100.f;
		up = rotation * up;
		right = rotation * right;
	}
	
	if (pitch != 0)
	{
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		position = target - view * 100.f;
		up = rotation * up;
		right = rotation * right;
	}
}

void Camera::Update(double dt, vector<Object*> objectVec)
{
	static const float CAMERA_SPEED = 50.f;
	static const float SPRINT_SPEED = 120.f;
	static const float LOOKING_SPEED = 20.f;
	static const float JUMPING_SPEED = 30.f;

	

	Vector3 boundCheckPos = position;

	if (Application::IsKeyPressed('T') && delay > 5)
	{
		if (flying == false)
		{
			flying = true;
			std::cout << "flying: on" << std::endl;
		}
		else
		{
			flying = false;
			std::cout << "flying: off" << std::endl;
		}
		delay = 0;
	}
	else if (delay < 5)
	{
		delay += (float)(5 * dt);
	}

	if (position.y > 10)
	{
		jumping = true;
	}
	else if (position.y < 0)
	{
		jumping = false;
		togJump = false;
	}
	if (Application::IsKeyPressed(VK_SPACE) && jumpDelay >= 10.0f && Singleton::getInstance()->pause == false)
	{
		if (togJump == false)
		{
			togJump = true;
		}
		else
		{
			togJump = false;
		}
		jumpDelay = 0;
	}
	else if (jumpDelay < 10.0f)
	{
		jumpDelay += (float)(13 * dt);
	}
	if (togJump)
	{
		if (!jumping)
		{
			position.y += (float)(JUMPING_SPEED * dt);
			target.y += (float)(JUMPING_SPEED * dt);
		}
		if (jumping)
		{
			position.y -= (float)(JUMPING_SPEED * dt);
			target.y -= (float)(JUMPING_SPEED * dt);
		}
	}

	if (Application::IsKeyPressed('W') && Singleton::getInstance()->pause == false)
	{
		if (flying == true)
		{
			if (Application::IsKeyPressed(VK_SHIFT))
			{
				position += view * (float)(SPRINT_SPEED * dt);
				target += view * (float)(SPRINT_SPEED * dt);
			}
			else
			{
				position += view * (float)(CAMERA_SPEED * dt);
				target += view * (float)(CAMERA_SPEED * dt);
			}
		}
		else
		{
			boundCheckPos.x += view.x * (float)(CAMERA_SPEED * dt);
			boundCheckPos.z += view.z * (float)(CAMERA_SPEED * dt);
			if (bound(boundCheckPos) && collision(boundCheckPos))
			{
				if (Application::IsKeyPressed(VK_SHIFT))
				{
					position.x += view.x * (float)(SPRINT_SPEED * dt);
					position.z += view.z * (float)(SPRINT_SPEED * dt);
					target.x += view.x * (float)(SPRINT_SPEED * dt);
					target.z += view.z * (float)(SPRINT_SPEED * dt);
				}
				else
				{
					position.x += view.x * (float)(CAMERA_SPEED * dt);
					position.z += view.z * (float)(CAMERA_SPEED * dt);
					target.x += view.x * (float)(CAMERA_SPEED * dt);
					target.z += view.z * (float)(CAMERA_SPEED * dt);
				}
			}
		}
	}
	if (Application::IsKeyPressed('S') && Singleton::getInstance()->pause == false)
	{
		if (flying == true)
		{
			position -= view * (float)(CAMERA_SPEED * dt);
			target -= view * (float)(CAMERA_SPEED * dt);
		}
		else
		{
			boundCheckPos.x -= view.x * (float)(CAMERA_SPEED * dt);
			boundCheckPos.z -= view.z * (float)(CAMERA_SPEED * dt);
			if (bound(boundCheckPos) && collision(boundCheckPos))
			{
				if (Application::IsKeyPressed(VK_SHIFT))
				{
					position.x -= view.x * (float)(SPRINT_SPEED * dt);
					position.z -= view.z * (float)(SPRINT_SPEED * dt);
					target.x -= view.x * (float)(SPRINT_SPEED * dt);
					target.z -= view.z * (float)(SPRINT_SPEED * dt);
				}
				else
				{
					position.x -= view.x * (float)(CAMERA_SPEED * dt);
					position.z -= view.z * (float)(CAMERA_SPEED * dt);
					target.x -= view.x * (float)(CAMERA_SPEED * dt);
					target.z -= view.z * (float)(CAMERA_SPEED * dt);
				}
			}
		}
	}
	if (Application::IsKeyPressed('D') && Singleton::getInstance()->pause == false)
	{
		if (flying == true)
		{
			position += right * (float)(CAMERA_SPEED * dt);
			target += right * (float)(CAMERA_SPEED * dt);
		}
		else
		{
			boundCheckPos += right * (float)(CAMERA_SPEED * dt);
			if (bound(boundCheckPos) && collision(boundCheckPos))
			{
				position += right * (float)(CAMERA_SPEED * dt);
				target += right * (float)(CAMERA_SPEED * dt);
			}
		}
	}
	if (Application::IsKeyPressed('A') && Singleton::getInstance()->pause == false)
	{
		if (flying == true)
		{
			position -= right * (float)(CAMERA_SPEED * dt);
			target -= right * (float)(CAMERA_SPEED * dt);
		}
		else
		{
			boundCheckPos -= right * (float)(CAMERA_SPEED * dt);
			if (bound(boundCheckPos) && collision(boundCheckPos))
			{
				position -= right * (float)(CAMERA_SPEED * dt);
				target -= right * (float)(CAMERA_SPEED * dt);
			}
		}
	}

	if (Singleton::getInstance()->pause == false)
	{
		Application::MouseMove(mousex, mousey);
		float yaw = LOOKING_SPEED * dt * static_cast<float>((SCREEN_WIDTH / 2) - mousex);
		float pitch = LOOKING_SPEED * dt * static_cast<float>((SCREEN_HEIGHT / 2) - mousey);
		// --- YAW ---
		if (yaw)
		{
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			view = rotation * view;
			target = position + view;
			right = rotation * right;
			up = rotation * up;
		}
		// -----------
		// --- PITCH ---
		if (pitch)
		{
			right.Normalize();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			view = rotation * view;
			target = position + view;
			up = right.Cross(view);
		}
		// --------------
	}
	else
	{
		Application::MouseUI(mousex, mousey);

		Vector3 maxPos(1905, 700, 0);
		Vector3 minPos(1430, 350, 0);
		if ((maxPos.x > mousex && minPos.x < mousex) &&
			(maxPos.y > mousey && minPos.y <mousey))
		{
			if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
			{
				Singleton::getInstance()->buttonText = true;
			}
			else
				Singleton::getInstance()->buttonText = false;
		}
		else
		{
			Singleton::getInstance()->buttonText = false;
		}
	}
	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void Camera::Reset()
{
	position.Set(1, 0, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}