#ifndef ASSIGNMENT2_H
#define ASSIGNMENT2_H

#include "Scene.h"
#include "Camera2.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"

class Ass2 : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_CUBE = 0,
		GEO_AXES,
		GEO_GROUND,
		GEO_LIGHTBALL,
		GEO_CAPE1,
		GEO_CAPE2,
		GEO_CAPE3,
		GEO_CAPE4,
		GEO_NOSE,
		GEO_MOUTH1,
		GEO_MOUTH2,
		GEO_HEAD,
		GEO_ARM1,
		GEO_ARM2,
		GEO_ARM3,
		GEO_EYE1,
		GEO_EYE2,
		GEO_HAT1,
		GEO_HAT2,
		GEO_CROSS,
		GEO_EAR,
		GEO_HORN1,
		GEO_HORN2,
		GEO_BODY,
		GEO_PANT,
		GEO_TREE1,
		GEO_TREE2,
		GEO_CANDY,
		GEO_STICK,

		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_NUMLIGHTS,

		U_TOTAL,
	};

public:
	Ass2();
	~Ass2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
private:
	void RenderMesh(Mesh *mesh, bool enableLight);

	unsigned m_vertexArrayID;
	Mesh *meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	float armRotate;
	float upperJump;
	float lowerJump;
	float inputDelay = 0.0f;
	bool checkArm;
	bool smile;
	bool jumping;
	bool togjump;

	Camera2 camera;
	Light light[10];

	MS modelStack, viewStack, projectionStack;
};

#endif