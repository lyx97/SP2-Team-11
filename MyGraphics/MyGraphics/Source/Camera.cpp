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
	Singleton::getInstance()->MOUSE_SPEED = 20.f;
}

bool bound(Vector3& camPos)
{
	for (auto q : Object::objectMap)
	{
		if ((q.first->hitbox.maxPt.x > camPos.x) && (q.first->hitbox.minPt.x < camPos.x) &&
			(q.first->hitbox.maxPt.y > camPos.y) && (q.first->hitbox.minPt.y < camPos.y) &&
			(q.first->hitbox.maxPt.z > camPos.z) && (q.first->hitbox.minPt.z < camPos.z))
		{
			return false;
		}
		else
		{
		}
	}
	return true;
}

void Camera::Update(double dt)
{
	static const float CAMERA_SPEED = 200.f;
	static const float SPRINT_SPEED = 800.f;
	static const float JUMP_SPEED = 80.f;
	Vector3 boundCheckPos = position;

	if (boundCheckPos.y > 10)
	{
		ySpeed += GRAVITY * dt;
	}
	else
	{
		ySpeed = 0;
	}
	position.y -= ySpeed;
	target.y -= ySpeed;

	if (delay < 5)
	{
		delay += (float)(30 * dt);
	}

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

	if (Application::IsKeyPressed(VK_SPACE))
	{
		position.y += (float)(JUMP_SPEED * dt);
		target.y += (float)(JUMP_SPEED * dt);
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
			if (bound(boundCheckPos))
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
			if (bound(boundCheckPos))
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
			if (bound(boundCheckPos))
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
			if (bound(boundCheckPos))
			{
				position -= right * (float)(CAMERA_SPEED * dt);
				target -= right * (float)(CAMERA_SPEED * dt);
			}
		}
	}

	if (Singleton::getInstance()->pause == false)
	{
		Application::MouseMove(mousex, mousey);
		float yaw = Singleton::getInstance()->MOUSE_SPEED * dt * static_cast<float>((SCREEN_WIDTH / 2) - mousex);
		float pitch = Singleton::getInstance()->MOUSE_SPEED * dt * static_cast<float>((SCREEN_HEIGHT / 2) - mousey);

		// mouse speed
		if (Application::IsKeyPressed(VK_SUBTRACT) && Singleton::getInstance()->MOUSE_SPEED > 1 && delay > 5)
		{
			Singleton::getInstance()->MOUSE_SPEED--;
			delay = 0;
		}
		if (Application::IsKeyPressed(VK_ADD) && Singleton::getInstance()->MOUSE_SPEED < 50 && delay > 5)
		{
			Singleton::getInstance()->MOUSE_SPEED++;
			delay = 0;
		}
		// --- YAW ---
		if (yaw)
		{
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			view = rotation * view;
			target = position + view * 5;
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
			if (target.y - position.y > 4.0)
			{
				if (pitch < 0)
				{
					view = rotation * view;
				}
			}
			else if (target.y - position.y < -4.0)
			{
				if (pitch > 0)
				{
					view = rotation * view;
				}
			}
			else if (target.y - position.y  > -4.0 && target.y - position.y < 4.0)
			{
				view = rotation * view;
			}
			target = position + view * 5;
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
	position.Set(150, 7, 150);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}