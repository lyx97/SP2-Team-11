#ifndef ASSIGNMENT1_H
#define ASSIGNMENT1_H

#include "Scene.h"

class Ass1 : public Scene
{

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_TOTAL,
	};

	enum GEOMETRY_TYPE
	{
		GEO_LAKE = 0,
		GEO_GRASS,
		GEO_TREE,
		GEO_BOATBODY,
		GEO_BOATPOLE,
		GEO_BOATFLAG,
		GEO_SUN,
		GEO_FLARE,
		GEO_CLOUD,
		GEO_SHELL,
		GEO_SHELL2,
		GEO_SNAIL,

		NUM_GEOMETRY,
	};

public:
	Ass1();
	~Ass1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	unsigned m_vertexArrayID;
	unsigned m_vertexBuffer[NUM_GEOMETRY];
	unsigned m_colorBuffer[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	float rotateAngle;
	float translateX;
	float translateXflag;
	float snail;
	float snail1;
	float snail2;
	float snail3;
	float trail;
	float cloud;
	float scaleAll;
	float rotateAroundSun;
	float rotateEyes;
	bool blob;

};

#endif