#ifndef LIGHT_H
#define LIGHT_H

#include "Vertex.h"

struct Light
{
	enum LIGHT_TYPE
	{
		LIGHT_POINT = 0,
		LIGHT_DIRECTIONAL,
		LIGHT_SPOT,

		LIGHT_TYPE_LAST,
	};

	LIGHT_TYPE type;
	Vector3 spotDirection;
	float cosCutoff = 1.f;
	float cosInner = 1.f;
	float exponent = 1.f;

	Position position;
	Color color;
	float power;
	float kC, kL, kQ; // constant, linear, quadratic

	Light()
	{
		position.Set(0, 0, 0);
		color.Set(1, 1, 1);
		power = 1.0f;
		kC = kL = kQ = 0;
	}
};

#endif