#ifndef SP2SCENE2_H
#define SP2SCENE2_H

#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>
#include "Object.h"

using std::vector;

class SP2Scene2 : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_CUBE = 0,
		GEO_AXES,
		GEO_GROUND,
		GEO_LIGHTBALL,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_ATAT,
		GEO_PELICAN,
		GEO_ROCK,
		GEO_TEXT,
		GEO_HOUSE1,
		GEO_HOUSE2,
		GEO_ORE,
		GEO_IMAGES,
		GEO_GUN,
		GEO_SWORD,
		GEO_CROSSHAIR,
		GEO_HP_BAR_LOW,
		GEO_HP_BAR_MID,
		GEO_HP_BAR_HIGH,
		GEO_BORDER,
		GEO_NPC1,
		GEO_HITBOX,

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
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TOTAL,
	};
public:
	SP2Scene2();
	~SP2Scene2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
private:
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderSkybox();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderUI(Mesh* mesh, float size, float x, float y, float scaleX);
	Camera camera;
	Light light[10];

	unsigned m_vertexArrayID;
	Mesh *meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	std::string FPS;
	float inputDelay = 0.f;
	int hp = 100;
	bool hpMid = false;
	bool hpLow = false;

	vector<Vector3> rockpos;


	MS modelStack, viewStack, projectionStack;
};

#endif