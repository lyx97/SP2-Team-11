/****************************************************************************/
/*!
\file SP2Scene2.cpp
\author Jacob Yang Shen
\author Lee Yu Xian
\par email: 153488J\@nyp.edu.sg
\par email: 155118A\@nyp.edu.sg
\brief
Second Scene of this project
*/
/****************************************************************************/
#ifndef SP2SCENE2_H
#define SP2SCENE2_H

#include "Scene.h"
#include "Camera.h"
#include "SP2.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Object.h"
#include "Sound.h"

using std::vector;
using std::string;
using std::ifstream;

/****************************************************************************/
/*!
Class SP2Scene2:
\brief Defines a SP2Scene2 class and its methods
*/
/****************************************************************************/
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
		GEO_MOON,
		GEO_PAUSE_BG,
		GEO_PAUSE_BUTTONS,
		GEO_PAUSE_BUTTONS_HOVER,
		GEO_SHIPDISTANCETAB,
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
	void Dialogue(string filename);
	void Reset();
	void pause();
	int moonDistance;

	Camera camera;
	Light light[10];

	unsigned m_vertexArrayID;
	Mesh *meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	string FPS;
	float inputDelay = 0.f;
	float turning = 0.f;
	float rotation = 0.f;
	bool hpMid = false;
	bool hpLow = false;
	bool pelicanhit = false;
	bool death = false;
	int rockfreq = 300;

	int acceleration;
	int handling;
	int turningspeed;
	int repair;
	int hp;


	vector<Vector3> rockpos;

	Vector3 pelicanPos;
	Vector3 momentum;

	Object* rock;

	MS modelStack, viewStack, projectionStack;

	string new_line;
	vector<string>my_arr;

	Sound sound;
};

#endif