/******************************************************************************/
/*!
\file	Plane
\author Sri Sham Haran
\par
\brief
Struct to define the Plane
*/
/******************************************************************************/
#ifndef PLANE_H
#define PLANE_H

#include "Vector3.h"
/******************************************************************************/
/*!
Struct plane:
\brief	Defines the plane (ground quad)
*/
/******************************************************************************/
struct plane
{
	Vector3 planePos;

	Vector3 planeMin;

	Vector3 planeMax;
};

#endif /*PLANE_H*/