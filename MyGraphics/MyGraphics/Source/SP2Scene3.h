#ifndef SP2SCENE3_H
#define SP2SCENE3_H

#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>
#include "Object.h"
#include "Plane.h"
#include <map>
#include "Boss.h"
#include "Weapon.h"
#include "Bullet.h"

using std::map;
using std::vector;

class SP2Scene3 : public Scene
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
        GEO_BOSS,
        GEO_BOSS_HAND1,
        GEO_BOSS_HAND2,
        GEO_BOSS_LEG1,
        GEO_BOSS_LEG2,
		GEO_BOSS_ICON,
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
    SP2Scene3();
    ~SP2Scene3();

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
    void planeInit(bool reset = false);
    float angleBetween(Vector3 &vector1, Vector3 &vector2);
    float magnitude(Vector3 &vector);
    void planeLoader();
    float distanceBetween(Vector3 from, Vector3 to);
    void swordSet(bool init = false);
    Camera camera;
    Light light[10];

    unsigned m_vertexArrayID;
    Mesh *meshList[NUM_GEOMETRY];

    unsigned m_programID;
    unsigned m_parameters[U_TOTAL];

    int planeDistance = 0;

    std::string FPS;
    float inputDelay = 0.f;
    bool hpMid = false;
    bool hpLow = false;
    bool miningDisplay = false;
    float heldDelay = 0.f;
    float pickSword = 0.f;
    float swing = 20.f;

    MS modelStack, viewStack, projectionStack;

    map<int, plane> planeMap;
    plane startingPlane;
    plane currPlane;
    int landMaxX, landMinX, landMaxZ, landMinZ;
    int currPlaneKey;
    int treeFrequency;
    vector<Vector3> treePos;
    int spawnRadius;

    bool activateBattle = false;
    bool bossDead = false;
    Object* bossObj;
	Object* ground;
    Boss boss = Boss("Final Boss", 1000, Vector3(800, 0, 800), Vector3(30, 150, 40));
    float bossLookAtAngle;

    vector<Vector3> swordVec;
    vector<Object*> swordObjVec;
    float swordDrag = 180;
    Weapon* sword;
    Mtx44 swordRotation;
    float spinSword = 0.f;
    float swordOffset = 80.f;

    Vector3 cameraStore;
    Vector3 swordPos;

    bool swordAniDown = false;
    bool swordAniUp = false;

//    vector<Bullet> bossBullet;

//    vector<Vector3> treesPos;
};

#endif