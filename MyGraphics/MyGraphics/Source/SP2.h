#ifndef SP2_H
#define SP2_H
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Plane.h"
#include "Object.h"

using std::map;
using std::vector;
using std::string;
using std::ifstream;

class SP2 : public Scene
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
		GEO_TEXT,
		GEO_ORE,
		GEO_IMAGES,
		GEO_GUN,
		GEO_SWORD,
		GEO_CROSSHAIR,
		GEO_HP_BAR_LOW,
		GEO_HP_BAR_MID,
		GEO_HP_BAR_HIGH,
		GEO_MINING_BAR,
		GEO_BORDER,
        GEO_NPC1,
        GEO_NPC1_HAND1,
        GEO_NPC1_HAND2,
        GEO_NPC1_LEG1,
        GEO_NPC1_LEG2,
        GEO_TREE,
        GEO_GRASS,
        GEO_HITBOX,
		GEO_MESSAGEBOX,
		GEO_NPC1_ICON,
		GEO_QUEST,
		GEO_HELPER,

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
	SP2();
	~SP2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
private:
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderSkybox();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderUI(Mesh* mesh, float size, float x, float y, float scaleX, float rotatex, float rotatey, float rotatez, bool enableLight);
	void Dialogue(string filename);
    void planeLoader();
    void planeInit(bool reset = false);

	unsigned m_vertexArrayID;
	Mesh *meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	bool hpMid = false;
	bool hpLow = false;
	bool miningDisplay = false;
	bool switchWeapon = true;
	bool shipStatus = false;

	float inputDelay;
	float weaponDelay;
	float heldDelay = 0.f;
	float pickSword = 0.f;
	float swing = 20.f;

	int planeDistance = 0;
    int numPlanes = 2;
	int oreFrequency;
    int treeFrequency;
    int grassFrequency;
    int currPlaneKey;
	int landMaxX, landMinX, landMaxZ, landMinZ;
    int spawnRadius;
	int mission;
	int message;

	string FPS;

	Vector3 swordPos;
	Vector3 gunPos;

	string new_line;
	vector<string>my_arr;

	// ores and land generators
	vector<Vector3> orePos;
    vector<Vector3> treePos;
    vector<Vector3> grassPos;

    Vector3 npcPos;
	Vector3 shipPos;
	map<int,plane> planeMap;

	plane startingPlane;
    plane currPlane;

	// objects
	Object* ore;
	Object* NPC;
	Object* ship;
	Object* sword;
	Object* gun;
    Object* tree;
	Object* ground;

	// weapons
	Weapon* melee;
	Weapon* ranged;
	Weapon* fist;

	Camera camera;
	Light light[10];

	MS modelStack, viewStack, projectionStack;
};

#endif