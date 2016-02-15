#ifndef SCENE_3_H
#define SCENE_3_H

#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"

class Scene3 : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_CUBE = 0,
		GEO_QUAD,
		GEO_AXES,
		GEO_CIRCLE,
		GEO_RING,
		GEO_SPHERE,
		GEO_HEMI,
		GEO_CYLIN,

		NUM_GEOMETRY,
	};
	enum UNIFORM_TYPE
	{
		U_MVP = 0,

		U_TOTAL,
	};
public:
	Scene3();
	~Scene3();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
private:
	unsigned m_vertexArrayID;
	Mesh *meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	float rotateAngle;

	Camera camera;
};

#endif