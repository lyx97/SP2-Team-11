#ifndef PLANE_H
#define PLANE_H

#include "Vector3.h"

struct plane
{
	Vector3 planePos;

	Vector3 planeMin;

	Vector3 planeMax;
};

#endif /*PLANE_H*/