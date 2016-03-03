/****************************************************************************/
/*!
\file SP2Scene3.cpp
\author Teo Jia Hao
\author Sri Sham Haran
\author Lee Yu Xian
\par email: kelvinteojiahao\@hotmail.com
\par email: 155129R\@mymail.nyp.edu.sg
\par email: 155118A\@mymail.nyp.edu.sg
\brief
Class to define SP2Scene3
*/
/****************************************************************************/
#include "SP2Scene3.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "LoadTGA.h"

#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"

#include <sstream>
#include <iostream>
using std::cout;
using std::endl;
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")
/******************************************************************************/
/*!
\brief
Default constructor

\exception None
\return None
*/
/******************************************************************************/
SP2Scene3::SP2Scene3()
{
}
/******************************************************************************/
/*!
\brief
Default destructor

\exception None
\return None
*/
/******************************************************************************/
SP2Scene3::~SP2Scene3()
{
}
/******************************************************************************/
/*!
\brief
Initializes for any booleans/strings/floats and meshes

\exception None
\return None
*/
/******************************************************************************/
void SP2Scene3::Init()
{
	startDialogue = true;
	sound.playMusic("Music//scenario3.mp3");

    // Init VBO here
	Dialogue("Text//NPC.txt");
	Application::HideCursor();
    Singleton::getInstance()->pause = false;
    Singleton::getInstance()->buttonText = false;
    treeFrequency = 200;
    spawnRadius = 5000;
    inputDelay = 0.0f;
	message = 0;
    planeInit();
    swordSet(true);

    

    // Set background color to dark blue
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    //Enable depth buffer and depth testing
    glEnable(GL_DEPTH_TEST);

    //Enable back face culling
    glEnable(GL_CULL_FACE);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Default to fill mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Generate a default VAO for now
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
    m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
    m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
    m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
    m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
    m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
    m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
    m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
    m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
    m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
    m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
    m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
    m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
    m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
    m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
    m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
    m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
    m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
    m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
    m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
    m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
    m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
    m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
    m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
    m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
    glUseProgram(m_programID);

    light[0].type = Light::LIGHT_DIRECTIONAL;
    light[0].position.Set(0, 500, 0);
    light[0].color.Set(1, 1, 1);
    light[0].power = 0.3f;
    light[0].kC = 1.0f;
    light[0].kL = 0.01f;
    light[0].kQ = 0.001f;
    light[0].cosCutoff = cos(Math::DegreeToRadian(45));
    light[0].cosInner = cos(Math::DegreeToRadian(30));
    light[0].exponent = 3.f;
    light[0].spotDirection.Set(0.f, 1.f, 0.f);


    // Make sure you pass uniform parameters after glUseProgram()
    glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
    glUniform1i(m_parameters[U_NUMLIGHTS], 1);
    glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
    glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
    glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
    glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
    glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
    glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
    glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
    glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);


    Mtx44 projection;
    projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 5000.f);
    projectionStack.LoadMatrix(projection);

    //Initialize camera settings
    camera.Init(Vector3(0, 20, 0), Vector3(90, 0, 0), Vector3(0, 1, 0));
	Application::SetMousePosition(0, 0);

    meshList[GEO_AXES] = MeshBuilder::GenerateAxes("AXES", 500, 500, 500);

    meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("LIGHTBALL", Color(1, 1, 1), 10, 20);
    meshList[GEO_LIGHTBALL]->textureID = LoadTGA("Image//bullet.tga");

	meshList[GEO_CYLIN] = MeshBuilder::GenerateCylin("FIST", Color(0.3f, 0.3f, 0.3f), 36);
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("FIST", Color(1.f, 0.863f, 0.698f), 36);

    meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("GROUND", Color(0.3f, 0.3f, 0.3f), TexCoord(10, 10), 1, 1);
    meshList[GEO_GROUND]->textureID = LoadTGA("Image//planet2_land.tga");

    meshList[GEO_HOUSE2] = MeshBuilder::GenerateCube("WALLS", Color(0.3f, 0.3f, 0.3f));

    meshList[GEO_ORE] = MeshBuilder::GenerateOBJ("ORE", "OBJ//Ore.obj");
    meshList[GEO_ORE]->textureID = LoadTGA("Image//TinOre.tga");

    meshList[GEO_PELICAN] = MeshBuilder::GenerateOBJ("GUN", "OBJ//air.obj");
    meshList[GEO_PELICAN]->textureID = LoadTGA("Image//air_UV.tga");

    meshList[GEO_IMAGES] = MeshBuilder::GenerateQuad("images", Color(1, 1, 1), TexCoord(1, 1), 1, 1);
    meshList[GEO_IMAGES]->textureID = LoadTGA("Image//images.tga");

    meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateQuad("crosshair", Color(1, 1, 1), TexCoord(1, 1), 1, 1);
    meshList[GEO_CROSSHAIR]->textureID = LoadTGA("Image//crosshair.tga");

    meshList[GEO_HP_BAR_LOW] = MeshBuilder::GenerateOBJ("ORE", "OBJ//hp.obj");
    meshList[GEO_HP_BAR_LOW]->textureID = LoadTGA("Image//hp.tga");

    meshList[GEO_HP_BAR_MID] = MeshBuilder::GenerateOBJ("ORE", "OBJ//hp.obj");
    meshList[GEO_HP_BAR_MID]->textureID = LoadTGA("Image//hp_mid.tga");

    meshList[GEO_HP_BAR_HIGH] = MeshBuilder::GenerateOBJ("ORE", "OBJ//hp.obj");
    meshList[GEO_HP_BAR_HIGH]->textureID = LoadTGA("Image//hp_high.tga");

    meshList[GEO_BORDER] = MeshBuilder::GenerateOBJ("ORE", "OBJ//hp.obj");
    meshList[GEO_BORDER]->textureID = LoadTGA("Image//border.tga");

	meshList[GEO_BOSS_ICON] = MeshBuilder::GenerateQuad("bossIcon", Color(1, 1, 1), TexCoord(1, 1), 1, 1);
	meshList[GEO_BOSS_ICON]->textureID = LoadTGA("Image//bossIcon.tga");

    meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("FRONT", Color(0, 0, 0), TexCoord(1, 1), 1, 1);
    meshList[GEO_FRONT]->textureID = LoadTGA("Image//red_ft.tga");
    meshList[GEO_BACK] = MeshBuilder::GenerateQuad("BACK", Color(0, 0, 0), TexCoord(1, 1), 1, 1);
    meshList[GEO_BACK]->textureID = LoadTGA("Image//red_bk.tga");
    meshList[GEO_TOP] = MeshBuilder::GenerateQuad("TOP", Color(0, 0, 0), TexCoord(1, 1), 1, 1);
    meshList[GEO_TOP]->textureID = LoadTGA("Image//red_up.tga");
    meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("BOTTOM", Color(0, 0, 0), TexCoord(1, 1), 1, 1);
    meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//red_dn.tga");
    meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("RIGHT", Color(0, 0, 0), TexCoord(1, 1), 1, 1);
    meshList[GEO_RIGHT]->textureID = LoadTGA("Image//red_rt.tga");
    meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("LEFT", Color(0, 0, 0), TexCoord(1, 1), 1, 1);
    meshList[GEO_LEFT]->textureID = LoadTGA("Image//red_lf.tga");

    meshList[GEO_TEXT] = MeshBuilder::GenerateText("TEXT", 16, 16);
    meshList[GEO_TEXT]->textureID = LoadTGA("Image//font.tga");

    meshList[GEO_BOSS] = MeshBuilder::GenerateOBJ("BOSS", "OBJ//NPC1_MAIN.obj");
    meshList[GEO_BOSS]->textureID = LoadTGA("Image//NPC_Evil.tga");
    meshList[GEO_BOSS_HAND1] = MeshBuilder::GenerateOBJ("BOSS", "OBJ//NPC1_HAND1.obj");
    meshList[GEO_BOSS_HAND1]->textureID = LoadTGA("Image//NPC_Evil.tga");
    meshList[GEO_BOSS_HAND2] = MeshBuilder::GenerateOBJ("BOSS", "OBJ//NPC1_HAND2.obj");
    meshList[GEO_BOSS_HAND2]->textureID = LoadTGA("Image//NPC_Evil.tga");
    meshList[GEO_BOSS_LEG1] = MeshBuilder::GenerateOBJ("BOSS", "OBJ//NPC1_LEG1.obj");
    meshList[GEO_BOSS_LEG1]->textureID = LoadTGA("Image//NPC_Evil.tga");
    meshList[GEO_BOSS_LEG2] = MeshBuilder::GenerateOBJ("BOSS", "OBJ//NPC1_LEG2.obj");
    meshList[GEO_BOSS_LEG2]->textureID = LoadTGA("Image//NPC_Evil.tga");

	meshList[GEO_MESSAGEBOX] = MeshBuilder::GenerateQuad("MESSAGEBOX", Color(1, 1, 1), TexCoord(1, 1), 6, 3);
	meshList[GEO_MESSAGEBOX]->textureID = LoadTGA("Image//messageBox.tga");

	meshList[GEO_WIN_BG] = MeshBuilder::GenerateQuad("WinUI", Color(1, 1, 1), TexCoord(1, 1), 3, 3);
	meshList[GEO_WIN_BG]->textureID = LoadTGA("Image//background5.tga");

	meshList[GEO_PAUSE_BG] = MeshBuilder::GenerateQuad("PauseUI", Color(1, 1, 1), TexCoord(1, 1), 3, 4);
	meshList[GEO_PAUSE_BG]->textureID = LoadTGA("Image//background4.tga");

	meshList[GEO_PAUSE_BUTTONS] = MeshBuilder::GenerateQuad("pauseBotton", Color(1, 1, 1), TexCoord(1, 1), 8, 2);
	meshList[GEO_PAUSE_BUTTONS]->textureID = LoadTGA("Image//buttonDefault.tga");

	meshList[GEO_PAUSE_BUTTONS_HOVER] = MeshBuilder::GenerateQuad("pauseHover", Color(1, 1, 1), TexCoord(1, 1), 8, 2);
	meshList[GEO_PAUSE_BUTTONS_HOVER]->textureID = LoadTGA("Image//buttonHover.tga");

	meshList[GEO_STATS] = MeshBuilder::GenerateQuad("STATS", Color(1, 1, 1), TexCoord(1, 1), 3, 3);
	meshList[GEO_STATS]->textureID = LoadTGA("Image//background3.tga");

    meshList[GEO_TREE] = MeshBuilder::GenerateOBJ("TREE", "OBJ//tree.obj");
    meshList[GEO_TREE]->textureID = LoadTGA("Image//tree2.tga");


    meshList[GEO_SWORD] = MeshBuilder::GenerateOBJ("SWORD", "OBJ//sword.obj");
    meshList[GEO_SWORD]->textureID = LoadTGA("Image//sword.tga");

    meshList[GEO_GUN] = MeshBuilder::GenerateOBJ("GUN", "OBJ//gun3.obj");
    meshList[GEO_GUN]->textureID = LoadTGA("Image//gun3.tga");

    meshList[GEO_BOSS_SWORD] = MeshBuilder::GenerateOBJ("BOSS_SWORD", "OBJ//sword.obj");
    meshList[GEO_BOSS_SWORD]->textureID = LoadTGA("Image//bossSword.tga");

    meshList[GEO_HITBOX] = MeshBuilder::GenerateCube("HITBOX", Color(1, 0, 0));

    startingPlane.planePos = Vector3(0, 0, 0);
    startingPlane.planeMin = Vector3(0, 0, 0);
    startingPlane.planeMax = Vector3(300, 0, 300);

    for (auto q : treePos)
    {
        tree = new Object(Vector3(q.x, 5, q.z), Vector3(40, 100, 40), true);
    }


	melee = new Weapon(5 + (Singleton::getInstance()->oreCount * 2));
	ranged = new Weapon(3 + Singleton::getInstance()->oreCount);
	fist = new Weapon(1);
    Singleton::getInstance()->gotSword = true;
    Singleton::getInstance()->gotGun = true;

	ground = new Object(Vector3(camera.position.x, 10, camera.position.z), Vector3(500, 10, 500));
}
/******************************************************************************/
/*!
\brief
Handles all animation and interaction

\exception None
\return None
*/
/******************************************************************************/
void SP2Scene3::Update(double dt)
{
	if (Application::IsKeyPressed('1')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	if (!bossDead)
	{
		cout << melee->getDamage() << endl;
		if (startDialogue)
		{
			if (Application::IsKeyPressed('E') && inputDelay >= 1)
			{
				message++;
				inputDelay = 0;

				if (message > 2)
				{
					activateBattle = true;
					//message = 0;
					//inputDelay = 0;
				}
			}
		}

		planeDistance = sqrtf((cameraStore.x - camera.position.x) * (cameraStore.x - camera.position.x) + (cameraStore.y - camera.position.y) * (cameraStore.y - camera.position.y) + (cameraStore.z - camera.position.z) * (cameraStore.z - camera.position.z));
		roateQuest += (float)(40 * dt);

		if (Singleton::getInstance()->pause == true)
		{
			pause();

			if (Application::IsKeyPressed('O'))
			{
				Application::HideCursor();
				Application::SetMousePosition(0, 0);
				Singleton::getInstance()->pause = false;
			}
		}
		else
		{

			ground->setPos(Vector3(camera.position.x, 7, camera.position.z));

			if ((GetKeyState(VK_LBUTTON) & 0x100) != 0 && !swordAniDown && !swordAniUp)
			{
				swordAniDown = true;
			}
			if (swordAniDown)
			{
				Singleton::getInstance()->rotateSword += (float)(500 * dt);
				if (Singleton::getInstance()->rotateSword >= 120)
				{
					swordAniDown = false;
					swordAniUp = true;
				}
			}
			if (swordAniUp)
			{
				Singleton::getInstance()->rotateSword -= (float)(500 * dt);
				if (Singleton::getInstance()->rotateSword <= 20)
				{
					swordAniDown = false;
					swordAniUp = false;
				}
			}

			if (Application::IsKeyPressed(VK_LBUTTON) && !Singleton::getInstance()->handUp && !Singleton::getInstance()->handDown)
			{
				Singleton::getInstance()->handDown = true;
			}
			if (Singleton::getInstance()->handDown)
			{
				Singleton::getInstance()->rotateHand -= (float)(150 * dt);
				if (Singleton::getInstance()->rotateHand <= 30)
				{
					Singleton::getInstance()->handDown = false;
					Singleton::getInstance()->handUp = true;
				}
			}
			if (Singleton::getInstance()->handUp)
			{
				Singleton::getInstance()->rotateHand += (float)(150 * dt);
				if (Singleton::getInstance()->rotateHand >= 45)
				{
					Singleton::getInstance()->handDown = false;
					Singleton::getInstance()->handUp = false;
				}
			}

			if (Application::IsKeyPressed(VK_LBUTTON) && !Singleton::getInstance()->fistDown && !Singleton::getInstance()->fistUp)
			{
				sound.playSoundEffect2D("Music//sword.wav");
				Singleton::getInstance()->fistDown = true;
			}
			if (Singleton::getInstance()->fistDown)
			{
				Singleton::getInstance()->moveFist -= (float)(50 * dt);
				if (Singleton::getInstance()->moveFist <= 5)
				{
					Singleton::getInstance()->fistDown = false;
					Singleton::getInstance()->fistUp = true;
				}
			}
			if (Singleton::getInstance()->fistUp)
			{
				Singleton::getInstance()->moveFist += (float)(50 * dt);
				if (Singleton::getInstance()->moveFist >= 10)
				{
					Singleton::getInstance()->fistDown = false;
					Singleton::getInstance()->fistUp = false;
				}
			}
			Singleton::getInstance()->gotSword = true;
			for (auto q : Object::objectMap){

				for (vector<Bullet*>::iterator it = Bullet::bulletVec.begin(); it != Bullet::bulletVec.end();)
				{
					(*it)->pos += (*it)->dir * (*it)->speed * dt;
					if (boss.object->hitbox.isTouching((*it)->pos)){
						boss.health -= 0.1f;
					}
					if (distanceBetween((*it)->pos, boss.position) > 200){

						delete *it;
						it = Bullet::bulletVec.erase(it);

					}
					//else if (q.first->hitbox.isTouching((*it)->pos)){
					//    delete *it;
					//    it = Bullet::bulletVec.erase(it);

					//}
					else{
						it++;
					}

				}

				for (vector<Bullet*>::iterator it = Bullet::bossBulletVec.begin(); it != Bullet::bossBulletVec.end();)
				{
					(*it)->pos += (*it)->dir * (*it)->speed * dt;
					//if (distanceBetween(boss.position, camera.position) < 30){
					//    Bullet::bossBulletVec.clear();


					//}
					if (distanceBetween((*it)->position, boss.position) > 500){

						delete *it;
						it = Bullet::bossBulletVec.erase(it);

					}

					else if (distanceBetween((*it)->position, camera.position) < 90){

						Singleton::getInstance()->health -= 1;
						delete *it;
						it = Bullet::bossBulletVec.erase(it);

					}

					else{
						it++;
					}
				}

			}
			if (Application::IsKeyPressed(VK_RBUTTON) && !Singleton::getInstance()->gunAniDown && !Singleton::getInstance()->gunAniUp && Singleton::getInstance()->gotGun)
			{
				sound.playSoundEffect2D("Music//laser.wav");
				bullet = new Bullet(Vector3(camera.target), Vector3(1, 1, 1), Vector3(camera.view), 8);

				Singleton::getInstance()->gunAniDown = true;
			}
			if (Singleton::getInstance()->gunAniDown)
			{
				Singleton::getInstance()->rotateGun += (float)(500 * dt);
				if (Singleton::getInstance()->rotateGun >= 50)
				{
					Singleton::getInstance()->gunAniDown = false;
					Singleton::getInstance()->gunAniUp = true;
				}
			}
			if (Singleton::getInstance()->gunAniUp)
			{
				Singleton::getInstance()->rotateGun -= (float)(100 * dt);
				if (Singleton::getInstance()->rotateGun <= 20)
				{
					Singleton::getInstance()->gunAniDown = false;
					Singleton::getInstance()->gunAniUp = false;
				}
			}
			if (Application::IsKeyPressed('P'))
			{
				Application::ShowCursor();
				Singleton::getInstance()->pause = true;
			}

			if (Singleton::getInstance()->health <= 50) hpMid = true;
			if (Singleton::getInstance()->health <= 25) hpLow = true;
			if (Singleton::getInstance()->health <= 0) Singleton::getInstance()->health = 0;

			if (Singleton::getInstance()->health >= 50) hpMid = false;
			if (Singleton::getInstance()->health >= 25) hpLow = false;
			if (Singleton::getInstance()->health >= 100) Singleton::getInstance()->health = 100;
			if (Application::IsKeyPressed('R'))
			{
				bool reset = true;
				planeInit(reset);
			}
			if (Application::IsKeyPressed('Z'))
				light[0].type = Light::LIGHT_POINT;
			glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
			if (Application::IsKeyPressed('X'))
				light[0].type = Light::LIGHT_DIRECTIONAL;
			glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
			if (Application::IsKeyPressed('C'))
				light[0].type = Light::LIGHT_SPOT;
			glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);

			FPS = std::to_string(toupper(1 / dt));
			if (Application::IsKeyPressed('B'))
				activateBattle = true;
			planeLoader();
			if (activateBattle){
				swordSet(false);

				if (distanceBetween(boss.object->pos, camera.target) < 140){
					if (swordOffset > 7)
						swordOffset -= (float)(130 * dt);
					if (swordDrag >= 25)
						swordDrag -= (float)(80 * dt);
				}
				else{
					if (swordOffset < 80)
						swordOffset += (float)(120 * dt);
					if (swordDrag < 200)
						swordDrag += (float)(80 * dt);

					if (rand() % 7 == 0)
						bullet = new Bullet(boss.position + Vector3(0, 40, 0), Vector3(1, 1, 1), (camera.position - Vector3(0, 40, 0) - boss.position).Normalized(), 8, false);
				}

				if (spinSword < 90)
					spinSword += 1;
				else spinSword -= 180;
				boss.setPos(boss.position);
				if (boss.health <= 0)
				{
					Application::ShowCursor();
					bossDead = true;
					boss.object->objectMap.erase(boss.object);
					for (auto q : swordObjVec){
						q->objectMap.erase(q);
					}
					Bullet::bossBulletVec.clear();
				}
				for (auto q : swordObjVec){

					if ((q->hitbox.isTouching(camera.position)) && bossDead == false)
						Singleton::getInstance()->health -= 1;
					if (distanceBetween(q->pos, camera.position) < 12 && bossDead == false)
						Singleton::getInstance()->health -= 1;

				}
				if ((GetKeyState(VK_LBUTTON) & 0x100) && distanceBetween(boss.object->pos, camera.target) < 30)
				{
					if (Singleton::getInstance()->gotSword)
					{
						boss.health -= melee->getDamage();
					}
					else if (Singleton::getInstance()->gotGun)
					{
						boss.health -= ranged->getDamage();
					}
					else
					{
						boss.health -= fist->getDamage();
					}
				}
				if (boss.health > 1){
					if (distanceBetween(boss.position, camera.position) >= 30){
						if (boss.position.x <= camera.position.x + 40)
							boss.position.x += (float)(150 * dt);

						if (boss.position.x >= camera.position.x - 40)
							boss.position.x -= (float)(150 * dt);

						if (boss.position.z <= camera.position.z + 40)
							boss.position.z += (float)(150 * dt);

						if (boss.position.z >= camera.position.z - 40)
							boss.position.z -= (float)(150 * dt);
					}

				}
			}

			if (!Application::IsKeyPressed('E'))
			{
				heldDelay = 0;
				pickSword = 0;
				swordPos.y = 5;
				miningDisplay = false;
			}
			if (inputDelay <= 1.0f)
			{
				inputDelay += (float)(1 * dt);
			}
			if (activateBattle)
				camera.Update(dt);
			if (Singleton::getInstance()->health <= 0)
			{
				sound.stopMusic("Music//scenario3.mp3");
				Singleton::getInstance()->stateCheck = true;
				Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME3;
				Object::objectMap.clear();
				Singleton::getInstance()->health = 100;
			}
		}
	}
}
/******************************************************************************/
/*!
\brief
Main rendering

\exception None
\return None
*/
/******************************************************************************/
void SP2Scene3::Render()
{
    // Render VBO here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    viewStack.LoadIdentity();
    viewStack.LookAt(
        camera.position.x, camera.position.y, camera.position.z,
        camera.target.x, camera.target.y, camera.target.z,
        camera.up.x, camera.up.y, camera.up.z
        );

    Position lightPosition_cameraspace = viewStack.Top() *	light[0].position;
    glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);

    if (light[0].type == Light::LIGHT_DIRECTIONAL)
    {
        Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
        Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
    }
    else if (light[0].type == Light::LIGHT_SPOT)
    {
        Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
        Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
        glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
    }
    else
    {
        Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
    }

    for (auto q : Object::objectMap)
    {
        //Tree Render
        for (int j = 0; j < treePos.size(); ++j)
        {
            if ((q.first->pos.x == treePos[j].x) && (q.first->pos.z == treePos[j].z))
            {
                modelStack.PushMatrix();
                modelStack.Translate(treePos[j].x, 0, treePos[j].z);
                modelStack.Scale(20, 20, 20);
                RenderMesh(meshList[GEO_TREE], true);
                modelStack.PopMatrix();

            }
        }
    }
    

    for (auto q : Object::objectMap)
    {
        modelStack.PushMatrix();
      //  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        modelStack.Translate(q.first->pos.x, q.first->pos.y, q.first->pos.z);
        modelStack.Scale(q.first->size.x, q.first->size.y, q.first->size.z);
     //  RenderMesh(meshList[GEO_HITBOX], false);
      //  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        modelStack.PopMatrix();
    }

    modelStack.PushMatrix();
    modelStack.Translate(camera.position.x, camera.position.y + 80, camera.position.z);
    RenderSkybox();
    modelStack.PopMatrix();

    map<int, plane>::iterator iter;

    for (iter = planeMap.begin(); iter != planeMap.end(); ++iter)
    {
        modelStack.PushMatrix();
        modelStack.Translate(iter->second.planePos.x, 0, iter->second.planePos.z);
        modelStack.Rotate(-90, 1, 0, 0);
        modelStack.Scale(150, 150, 1);
        RenderMesh(meshList[GEO_GROUND], true);
        modelStack.PopMatrix();
    }

    if (boss.health >= 1){
        modelStack.PushMatrix();
        modelStack.Translate(boss.position.x, boss.position.y, boss.position.z);
        modelStack.Rotate(angleBetween(boss.position, camera.position), 0, 1, 0);

        modelStack.Scale(20, 20, 20);
        modelStack.PushMatrix();
        RenderMesh(meshList[GEO_BOSS], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        RenderMesh(meshList[GEO_BOSS_HAND1], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        RenderMesh(meshList[GEO_BOSS_HAND2], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        RenderMesh(meshList[GEO_BOSS_LEG1], true);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        RenderMesh(meshList[GEO_BOSS_LEG2], true);
        modelStack.PopMatrix();

        modelStack.PopMatrix();

        for (auto q : Bullet::bulletVec)
        {
            modelStack.PushMatrix();
            modelStack.Translate(q->pos.x, q->pos.y, q->pos.z);
            RenderMesh(meshList[GEO_LIGHTBALL], false);
            modelStack.PopMatrix();
        }

        for (auto q : Bullet::bossBulletVec)
        {
            modelStack.PushMatrix();
            modelStack.Translate(q->pos.x, q->pos.y, q->pos.z);
            RenderMesh(meshList[GEO_LIGHTBALL], false);
            modelStack.PopMatrix();
        }

        modelStack.PushMatrix();
        //modelStack.Translate(boss.position.x, boss.position.y, boss.position.z);
        // modelStack.Rotate(spinSword, 0, 1, 0);
        // modelStack.Translate(-boss.position.x, -boss.position.y, -boss.position.z);
        for (auto q : swordVec)
        {
            modelStack.PushMatrix();

            modelStack.Translate(q.x, q.y, q.z);
            modelStack.Scale(6, 6, 6);
            modelStack.Rotate(spinSword, 0, 1, 0);

            RenderMesh(meshList[GEO_BOSS_SWORD], true);
            modelStack.PopMatrix();
        }

        modelStack.PopMatrix();
    
		if (Singleton::getInstance()->gotSword && Singleton::getInstance()->gotGun)
		{
			RenderUI(meshList[GEO_SWORD], 13, 75, -7, 1, 0, -60, Singleton::getInstance()->rotateSword, true);
			RenderUI(meshList[GEO_GUN], 120, 10, 5, 1, -5, 80, Singleton::getInstance()->rotateGun, true);
		}
		else
		{
			if (Singleton::getInstance()->gotSword)
			{
				RenderUI(meshList[GEO_SWORD], 13, 75, -7, 1, 0, -60, Singleton::getInstance()->rotateSword, true);
			}
			else if (Singleton::getInstance()->gotGun)
			{
				RenderUI(meshList[GEO_GUN], 120, 70, 5, 1, -5, 100, Singleton::getInstance()->rotateGun, true);
			}
			else
			{
				// right
				RenderUI(meshList[GEO_CIRCLE], 7, 60, Singleton::getInstance()->moveFist, 1, 90, 0, 0, false);
				RenderUI(meshList[GEO_CYLIN], 15, 70, 0, 1, 0, 0, -Singleton::getInstance()->rotateHand, false);
				// left
				RenderUI(meshList[GEO_CIRCLE], 7, 20, Singleton::getInstance()->moveFist, 1, 90, 0, 0, false);
				RenderUI(meshList[GEO_CYLIN], 15, 10, 0, 1, 0, 0, Singleton::getInstance()->rotateHand, false);
			}
		}

        if (boss.health > 0 && boss.health <= 100)
            RenderTextOnScreen(meshList[GEO_TEXT], "FINISH HIM", Color(1, 0, 0), 2, 60, 10);

		RenderUI(meshList[GEO_HP_BAR_LOW], 6, 8, 55, boss.health / 100, 0, 0, 0, false);
		RenderUI(meshList[GEO_BORDER], 6, 8, 55, 10, 0, 0, 0, false);
		RenderUI(meshList[GEO_BOSS_ICON], 3, 5, 55, 1, 0, 0, 0, false);
    }
    
    //t->r->s
    //RenderMesh(meshList[GEO_AXES], false);

    modelStack.PushMatrix();
    modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
    //RenderMesh(meshList[GEO_LIGHTBALL], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(
        camera.target.x,
        camera.target.y,
        camera.target.z
        );
    modelStack.Scale(0.1f, 0.1f, 0.1f);
    //RenderMesh(meshList[GEO_LIGHTBALL], true);
    modelStack.PopMatrix();

    if (!hpMid && !hpLow)
		RenderUI(meshList[GEO_HP_BAR_HIGH], 2, 10, 10, Singleton::getInstance()->health / 10, 0, 0, 0, false);

    if (hpMid && !hpLow)
		RenderUI(meshList[GEO_HP_BAR_MID], 2, 10, 10, Singleton::getInstance()->health / 10, 0, 0, 0, false);

    if (hpMid && hpLow)
		RenderUI(meshList[GEO_HP_BAR_LOW], 2, 10, 10, Singleton::getInstance()->health / 10, 0, 0, 0, false);

    RenderUI(meshList[GEO_BORDER], 2, 10, 10, 10, 0, 0, 0, false);
    RenderTextOnScreen(meshList[GEO_TEXT], "HP: ", Color(0, 1, 0), 2, 5, 10);

	int j = 14;
	switch (message)
	{
	case 0:
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		RenderUI(meshList[GEO_BOSS_ICON], 5, 13, 10, 1, 0, 0, 0, false);

		for (int i = 27; i <= 28; ++i)
		{
			j -= 2;
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[i], Color(1, 1, 0), 1.3, 23, j);
		}
		break;
	case 1:
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		RenderUI(meshList[GEO_BOSS_ICON], 5, 13, 10, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[29], Color(1, 1, 0), 1.5, 23, 10);
		break;
	case 2:
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		RenderUI(meshList[GEO_BOSS_ICON], 5, 13, 10, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[30], Color(1, 1, 0), 1.3, 23, 10);
		break;
	default:
		break;
	}

    RenderUI(meshList[GEO_CROSSHAIR], 1, 40, 30, 1, 0, 0, 0, false);
    RenderTextOnScreen(meshList[GEO_TEXT], FPS + " FPS", Color(0, 1, 0), 1, 1, 1);	// fps
    RenderTextOnScreen(meshList[GEO_TEXT], "POSITION X: " + std::to_string(camera.position.x), Color(0, 0, 0), 1, 1, 50);
    RenderTextOnScreen(meshList[GEO_TEXT], "POSITION Z: " + std::to_string(camera.position.z), Color(0, 0, 0), 1, 1, 48);
	RenderTextOnScreen(meshList[GEO_TEXT], "Mouse X: " + std::to_string(Singleton::getInstance()->mousex), Color(0, 0, 0), 1, 1, 46);
	RenderTextOnScreen(meshList[GEO_TEXT], "Mouse Z: " + std::to_string(Singleton::getInstance()->mousey), Color(0, 0, 0), 1, 1, 44);
    RenderTextOnScreen(meshList[GEO_TEXT], "Pause Check" + std::to_string(Singleton::getInstance()->pause), Color(0, 0, 0), 1, 1, 38);

    RenderTextOnScreen(meshList[GEO_TEXT], "Mouse Speed: " + std::to_string(toupper(Singleton::getInstance()->MOUSE_SPEED)), Color(0, 0, 0), 1, 1, 28);
    if (Singleton::getInstance()->buttonText == true)
        RenderTextOnScreen(meshList[GEO_TEXT], "Button Click", Color(0, 0, 0), 1, 40, 25);

	if (Application::IsKeyPressed('I'))
	{
		RenderUI(meshList[GEO_STATS], 5, 40, 30, 1.3, 0, 0, 0, false);

		RenderUI(meshList[GEO_SWORD], 1, 28, 30, 1, 0, roateQuest, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], "Melee damage: " + std::to_string(melee->getDamage()), Color(1, 1, 1), 1, 35, 30);

		RenderUI(meshList[GEO_GUN], 10, 28, 25, 1, 0, roateQuest, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], "Ranged damage: " + std::to_string(ranged->getDamage()), Color(1, 1, 1), 1, 35, 25);

	}

	if (Singleton::getInstance()->pause == true)
		pause();
	if (bossDead == true)
	{
		win();
	}
}
/******************************************************************************/
/*!
\brief
Renders skybox

\exception None
\return None
*/
/******************************************************************************/
void SP2Scene3::RenderSkybox()
{
    modelStack.PushMatrix();
    modelStack.Scale(0.8f, 0.8f, 0.8f);

    modelStack.PushMatrix();
    modelStack.Translate(0, 0, 997.f);
    modelStack.Rotate(180, 0, 1, 0);
    modelStack.Scale(1000, 1000, 1000);
    RenderMesh(meshList[GEO_FRONT], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(0, 0, -997.f);
    modelStack.Scale(1000, 1000, 1000);
    RenderMesh(meshList[GEO_BACK], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(0, -997.f, 0);
    modelStack.Rotate(-90, 1, 0, 0);
    modelStack.Rotate(-90, 0, 0, 1);
    modelStack.Scale(1000, 1000, 1000);
    RenderMesh(meshList[GEO_BOTTOM], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(0, 997.f, 0);
    modelStack.Rotate(90, 1, 0, 0);
    modelStack.Rotate(90, 0, 0, 1);
    modelStack.Scale(1000, 1000, 1000);
    RenderMesh(meshList[GEO_TOP], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(-997.f, 0, 0);
    modelStack.Rotate(90, 0, 1, 0);
    modelStack.Scale(1000, 1000, 1000);
    RenderMesh(meshList[GEO_LEFT], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(997.f, 0, 0);
    modelStack.Rotate(-90, 0, 1, 0);
    modelStack.Scale(1000, 1000, 1000);
    RenderMesh(meshList[GEO_RIGHT], false);
    modelStack.PopMatrix();

    modelStack.PopMatrix();

}
/******************************************************************************/
/*!
\brief
Pushes in the Dialogue into array

\exception None
\return None
*/
/******************************************************************************/
void SP2Scene3::Dialogue(string filename)
{
	ifstream myfile(filename.c_str());
	string line;

	while (std::getline(myfile, line))
	{
		new_line = line + "\n";
		cout << new_line;
		my_arr.push_back(new_line);
	}
}
/******************************************************************************/
/*!
\brief
Renders mesh

\exception None
\return None
*/
/******************************************************************************/
void SP2Scene3::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();

		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once 
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
/******************************************************************************/
/*!
\brief
Renders text

\exception None
\return None
*/
/******************************************************************************/
void SP2Scene3::RenderText(Mesh* mesh, std::string text, Color color)
{
    if (!mesh || mesh->textureID <= 0) //Proper error check
        return;

    glDisable(GL_DEPTH_TEST);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
    glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->textureID);
    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    for (unsigned i = 0; i < text.length(); ++i)
    {
        Mtx44 characterSpacing;
        characterSpacing.SetToTranslation(i * 1.5f, 0, 0); //1.0f is the spacing of each character, you may change this value
        Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
        glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

        mesh->Render((unsigned)text[i] * 6, 6);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
    glEnable(GL_DEPTH_TEST);
}
/******************************************************************************/
/*!
\brief
Renders text on screen

\exception None
\return None
*/
/******************************************************************************/
void SP2Scene3::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
    if (!mesh || mesh->textureID <= 0) //Proper error check
        return;

    glDisable(GL_DEPTH_TEST);
    Mtx44 ortho;
    ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
    projectionStack.PushMatrix();
    projectionStack.LoadMatrix(ortho);
    viewStack.PushMatrix();
    viewStack.LoadIdentity(); //No need camera for ortho mode
    modelStack.PushMatrix();
    modelStack.LoadIdentity(); //Reset modelStack
    modelStack.Translate(x, y, 0);
    modelStack.Scale(size, size, size);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
    glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->textureID);
    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    for (unsigned i = 0; i < text.length(); ++i)
    {
        Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.8f, 0, 0); //1.0f is the spacing of each character, you may change this value
        Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
        glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

        mesh->Render((unsigned)text[i] * 6, 6);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);
}
/******************************************************************************/
/*!
\brief
Renders obj and targa on screen

\param mesh
Takes in which mesh to render out
\param size
Scale of the mesh to render out
\param x and y
The position of the mesh
\param scaleX
Scale of the mesh
\param rotatex, rotatey, rotatez
Rotation of the Mesh
\param
Mesh with light

\exception None
\return None
*/
/******************************************************************************/
void SP2Scene3::RenderUI(Mesh* mesh, float size, float x, float y, float scaleX, float rotatex, float rotatey, float rotatez, bool enableLight)
{
    glDisable(GL_DEPTH_TEST);
    Mtx44 ortho;
    ortho.SetToOrtho(0, 80, 0, 60, -100, 100); //size of screen UI
    projectionStack.PushMatrix();
    projectionStack.LoadMatrix(ortho);
    viewStack.PushMatrix();
    viewStack.LoadIdentity(); //No need camera for ortho mode
    modelStack.PushMatrix();
    modelStack.LoadIdentity(); //Reset modelStack
    modelStack.Translate(x, y, 1);
    modelStack.Rotate(rotatex, 1, 0, 0);
    modelStack.Rotate(rotatey, 0, 1, 0);
    modelStack.Rotate(rotatez, 0, 0, 1);
    modelStack.Scale(size, size, size);
    modelStack.Scale(scaleX, 1, 1);
    if (enableLight)
    {
        glUniform1i(m_parameters[U_LIGHTENABLED], 1);
    }
    else
    {
        glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    }
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

    Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
    glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

    mesh->Render();

    glBindTexture(GL_TEXTURE_2D, 0);
    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);
}

/******************************************************************************/
/*!
\brief
Initializes or resets planes/ground quads

\param reset
Check if method is being used to reset the planes or not

\exception None
\return None
*/
/******************************************************************************/
void SP2Scene3::planeInit(bool reset){
    if (reset == false){
        plane startingPlane;

        //Plane[0]                                               
        startingPlane.planeMax = Vector3(-300, 0, 300);
        startingPlane.planeMin = Vector3(-450, 0, 150);
        startingPlane.planePos = startingPlane.planeMin;
        planeMap.insert(std::pair<int, plane>(0, startingPlane));

        //Plane[1]                                               
        startingPlane.planeMax = Vector3(0, 0, 300);
        startingPlane.planeMin = Vector3(-150, 0, 150);
        startingPlane.planePos = startingPlane.planeMin;
        planeMap.insert(std::pair<int, plane>(1, startingPlane));

        //Plane[2]                                               
        startingPlane.planeMax = Vector3(300, 0, 300);
        startingPlane.planeMin = Vector3(150, 0, 150);
        startingPlane.planePos = startingPlane.planeMin;
        planeMap.insert(std::pair<int, plane>(2, startingPlane));
        //    3x3 map grid (for reference)
        //Plane[3]                                                                          //   *------* *------* *------*
        startingPlane.planeMax = Vector3(-300, 0, 0);                                       //   |      | |      | |      |     
        startingPlane.planeMin = Vector3(-450, 0, -150);                                    //   |  0   | |   1  | |   2  |    
        startingPlane.planePos = startingPlane.planeMin;                                    //   *------* *------* *------*    
        planeMap.insert(std::pair<int, plane>(3, startingPlane));                           //   *------* *------* *------*    
        //   |      | |  (p) | |      |    
        //Plane[4]                                                                          //   |   3  | |   4  | |   5  |    
        startingPlane.planeMax = Vector3(0, 0, 0);                                          //   *------* *------* *------*    
        startingPlane.planeMin = Vector3(-150, 0, -150);                                    //   *------* *------* *------*    
        startingPlane.planePos = startingPlane.planeMin;                                    //   |      | |      | |      |    
        planeMap.insert(std::pair<int, plane>(4, startingPlane));                           //   |   6  | |   7  | |  8   |    
        //   *------* *------* *------*    
        //Plane[5]                                       
        startingPlane.planeMax = Vector3(300, 0, 0);
        startingPlane.planeMin = Vector3(150, 0, -150);
        startingPlane.planePos = startingPlane.planeMin;
        planeMap.insert(std::pair<int, plane>(5, startingPlane));

        //Plane[6]                                               
        startingPlane.planeMax = Vector3(-300, 0, -300);
        startingPlane.planeMin = Vector3(-450, 0, -450);
        startingPlane.planePos = startingPlane.planeMin;
        planeMap.insert(std::pair<int, plane>(6, startingPlane));

        //Plane[7]                                               
        startingPlane.planeMax = Vector3(0, 0, -300);
        startingPlane.planeMin = Vector3(-150, 0, -450);
        startingPlane.planePos = startingPlane.planeMin;
        planeMap.insert(std::pair<int, plane>(7, startingPlane));

        //Plane[8]                                               
        startingPlane.planeMax = Vector3(300, 0, -300);
        startingPlane.planeMin = Vector3(150, 0, -450);
        startingPlane.planePos = startingPlane.planeMin;
        planeMap.insert(std::pair<int, plane>(8, startingPlane));

        //Tree spawning
        for (int loop = 0; loop < treeFrequency / 4; loop++)
        {
            treePos.push_back(Vector3(rand() % spawnRadius, 0, rand() % spawnRadius));
        }
        for (int loop = 0; loop < treeFrequency / 4; loop++)
        {
            treePos.push_back(Vector3((rand() % spawnRadius) - spawnRadius, 0, rand() % spawnRadius));
        }
        for (int loop = 0; loop < treeFrequency / 4; loop++)
        {
            treePos.push_back(Vector3((rand() % spawnRadius) - spawnRadius, 0, (rand() % spawnRadius) - spawnRadius));
        }

        for (int loop = 0; loop < treeFrequency / 4; loop++)
        {
            treePos.push_back(Vector3(rand() % spawnRadius + 0, 0, (rand() % spawnRadius) - spawnRadius));
        }

    }
    else{
        plane startingPlane;

        //Plane[0]                                               
        startingPlane.planeMax = Vector3(-300, 0, 300);
        startingPlane.planeMin = Vector3(-450, 0, 150);
        startingPlane.planePos = startingPlane.planeMin;
        planeMap[0] = startingPlane;

        //Plane[1]                                               
        startingPlane.planeMax = Vector3(0, 0, 300);
        startingPlane.planeMin = Vector3(-150, 0, 150);
        startingPlane.planePos = startingPlane.planeMin;
        planeMap[1] = startingPlane;

        //Plane[2]                                               
        startingPlane.planeMax = Vector3(300, 0, 300);
        startingPlane.planeMin = Vector3(150, 0, 150);
        startingPlane.planePos = startingPlane.planeMin;
        planeMap[2] = startingPlane;
        //    3x3 map grid (for reference)
        //Plane[3]                                                                          //   *------* *------* *------*
        startingPlane.planeMax = Vector3(-300, 0, 0);                                       //   |      | |      | |      |     
        startingPlane.planeMin = Vector3(-450, 0, -150);                                    //   |  0   | |   1  | |   2  |    
        startingPlane.planePos = startingPlane.planeMin;                                    //   *------* *------* *------*    
        planeMap[3] = startingPlane;                                                        //   *------* *------* *------*    
        //   |      | |  (p) | |      |    
        //Plane[4]                                                                          //   |   3  | |   4  | |   5  |    
        startingPlane.planeMax = Vector3(0, 0, 0);                                          //   *------* *------* *------*    
        startingPlane.planeMin = Vector3(-150, 0, -150);                                    //   *------* *------* *------*    
        startingPlane.planePos = startingPlane.planeMin;                                    //   |      | |      | |      |    
        planeMap[4] = startingPlane;                                                        //   |   6  | |   7  | |  8   |    
        //   *------* *------* *------*    
        //Plane[5]                                       
        startingPlane.planeMax = Vector3(300, 0, 0);
        startingPlane.planeMin = Vector3(150, 0, -150);
        startingPlane.planePos = startingPlane.planeMin;
        planeMap[5] = startingPlane;

        //Plane[6]                                               
        startingPlane.planeMax = Vector3(-300, 0, -300);
        startingPlane.planeMin = Vector3(-450, 0, -450);
        startingPlane.planePos = startingPlane.planeMin;
        planeMap[6] = startingPlane;

        //Plane[7]                                               
        startingPlane.planeMax = Vector3(0, 0, -300);
        startingPlane.planeMin = Vector3(-150, 0, -450);
        startingPlane.planePos = startingPlane.planeMin;
        planeMap[7] = startingPlane;

        //Plane[8]                                               
        startingPlane.planeMax = Vector3(300, 0, -300);
        startingPlane.planeMin = Vector3(150, 0, -450);
        startingPlane.planePos = startingPlane.planeMin;
        planeMap[8] = startingPlane;

    }
}
/******************************************************************************/
/*!
\brief
Updates the position of the plane/ ground quad based on player position

\exception None
\return None
*/
/******************************************************************************/
void SP2Scene3::planeLoader(){

    for (auto iter : planeMap){

        if ((camera.position.x >= iter.second.planeMin.x && camera.position.x <= iter.second.planeMax.x) && (camera.position.z >= iter.second.planeMin.z && camera.position.z <= iter.second.planeMax.z)){

            currPlane = iter.second;
            currPlaneKey = iter.first;
        }

    }

    planeMap[4] = planeMap[currPlaneKey];
    planeMap[1].planeMin = planeMap[1].planePos = planeMap[4].planeMin + Vector3(0, 0, 150);
    planeMap[1].planeMax = planeMap[1].planeMin + Vector3(150, 0, 150);
    planeMap[0].planeMin = planeMap[0].planePos = planeMap[1].planeMin - Vector3(150, 0, 0);
    planeMap[0].planeMax = planeMap[0].planeMin + Vector3(150, 0, 150);
    planeMap[2].planeMin = planeMap[2].planePos = planeMap[1].planeMin + Vector3(150, 0, 0);
    planeMap[2].planeMax = planeMap[2].planeMin + Vector3(150, 0, 150);
    planeMap[3].planeMin = planeMap[3].planePos = planeMap[4].planeMin - Vector3(150, 0, 0);
    planeMap[3].planeMax = planeMap[3].planeMin + Vector3(150, 0, 150);
    planeMap[5].planeMin = planeMap[5].planePos = planeMap[4].planeMin + Vector3(150, 0, 0);
    planeMap[5].planeMax = planeMap[5].planeMin + Vector3(150, 0, 150);
    planeMap[6].planeMin = planeMap[6].planePos = planeMap[4].planeMin - Vector3(150, 0, 150);
    planeMap[6].planeMax = planeMap[6].planeMin + Vector3(150, 0, 150);
    planeMap[7].planeMin = planeMap[7].planePos = planeMap[6].planeMin + Vector3(150, 0, 0);
    planeMap[7].planeMax = planeMap[7].planeMin + Vector3(150, 0, 150);
    planeMap[8].planeMin = planeMap[8].planePos = planeMap[7].planeMin + Vector3(150, 0, 0);
    planeMap[8].planeMax = planeMap[8].planeMin + Vector3(150, 0, 150);

}

/******************************************************************************/
/*!
\brief
calculate distance from one point to another

\param from
First vector/intial vector to calculate using

\param to
Second vector/end vector to calculate using

\exception None
\return float value of the distance obtained from the vectors that were passed in
*/
/******************************************************************************/

float SP2Scene3::distanceBetween(Vector3 from, Vector3 to){
    return sqrt((pow(to.x - from.x, 2)) + (pow(to.z - from.z, 2)));
}
/******************************************************************************/
/*!
\brief
calculate magnitude of a vector

\param vector
Vector to calculate magnitude of

\exception None
\return float value of the magnitude
*/
/******************************************************************************/
float SP2Scene3::magnitude(Vector3 &vector){
    return sqrt((pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2)));
}
/******************************************************************************/
/*!
\brief
calculate distance from one point to another

\param vector1
First vector/intial vector to calculate using

\param vector2
Second vector/end vector to calculate using

\exception None
\return float value of the angle between the vectors that were passed in
*/
/******************************************************************************/
float SP2Scene3::angleBetween(Vector3 &vector1, Vector3 &vector2){
    float theta = Math::RadianToDegree(acos((vector1.Dot(vector2)) / ((magnitude(vector1)) * (magnitude(vector2))))) * 8;
    if (theta < 0)
        theta = -theta;
    return theta;
}

/******************************************************************************/
/*!
\brief
Set positions of swords relative to 

\param from
First vector/intial vector to calculate using

\param to
Second vector/end vector to calculate using

\exception None
\return float value of the distance obtained from the vectors that were passed in
*/
/******************************************************************************/
void SP2Scene3::swordSet(bool init)
{
    float offset = swordDrag;
    if (init == true){
        swordVec.push_back(Vector3(0, 0, offset ) + boss.position);
        swordVec.push_back(Vector3(offset , 0, offset ) + boss.position);
        swordVec.push_back(Vector3(-offset , 0, offset ) + boss.position);
        swordVec.push_back(Vector3(-offset , 0, 0) + boss.position);
        swordVec.push_back(Vector3(offset , 0, 0) + boss.position);
        swordVec.push_back(Vector3(-offset , 0, -offset ) + boss.position);
        swordVec.push_back(Vector3(0, 0, -offset ) + boss.position);
        swordVec.push_back(Vector3(offset , 0, -offset ) + boss.position);
        for (auto q : swordVec){
            swordObjVec.push_back(new Object(q, Vector3(20, 80, 20)));
        }
    }
    else{
        if (boss.health >= 1){
            swordVec[0] = Vector3(0, 0, offset) + boss.position + Vector3(0, swordOffset, 0);
            swordVec[1] = Vector3(offset, 0, offset) + boss.position + Vector3(0, swordOffset, 0);
            swordVec[2] = Vector3(-offset, 0, offset) + boss.position + Vector3(0, swordOffset, 0);
            swordVec[3] = Vector3(-offset, 0, 0) + boss.position + Vector3(0, swordOffset, 0);
            swordVec[4] = Vector3(offset, 0, 0) + boss.position + Vector3(0, swordOffset, 0);
            swordVec[5] = Vector3(-offset, 0, -offset) + boss.position + Vector3(0, swordOffset, 0);
            swordVec[6] = Vector3(0, 0, -offset) + boss.position + Vector3(0, swordOffset, 0);
            swordVec[7] = Vector3(offset, 0, -offset) + boss.position + Vector3(0, swordOffset, 0);
            int loop = 0;

            for (auto q : swordObjVec){
                *q = Object(swordVec[loop], q->size);
                loop++;
            }
        }
    }
}/******************************************************************************/
/*!
\brief
Render pause

\exception None
\return None
*/
/******************************************************************************/
void SP2Scene3::pause()
{
	RenderUI(meshList[GEO_PAUSE_BG], 5, 40, 30, 1.3, 0, 0, 0, false);

	/////////////////////////////
	//     MAINMENU BUTTON     //
	/////////////////////////////
	if ((1152 * SCREEN_WIDTH / 1920 > Singleton::getInstance()->mousex && 767 * SCREEN_WIDTH / 1920 < Singleton::getInstance()->mousex) &&
		(468 * SCREEN_HEIGHT / 1080 > Singleton::getInstance()->mousey && 398 * SCREEN_HEIGHT / 1080 <Singleton::getInstance()->mousey))
	{
		//MOUSE CLICK
		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 36, 1, 0, 0, 0, false);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[35], Color(1, 0, 0), 1, 37, 36);
			Singleton::getInstance()->stateCheck = true;
			Singleton::getInstance()->program_state = Singleton::PROGRAM_MENU;
			Object::objectMap.clear();
		}
		//MOUSE HOVER
		else
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 36, 1, 0, 0, 0, false);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[35], Color(1, 0, 0), 1, 37, 36);
		}
	}
	//DEFAULT
	else
	{
		RenderUI(meshList[GEO_PAUSE_BUTTONS], 1, 40, 36, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[35], Color(1, 1, 1), 1, 37, 36);
	}
	///////////////////////////
	//     RESUME BUTTON     //
	//////////////////////////
	if ((1152 * SCREEN_WIDTH / 1920 > Singleton::getInstance()->mousex && 767 * SCREEN_WIDTH / 1920 < Singleton::getInstance()->mousex) &&
		(575 * SCREEN_HEIGHT / 1080 > Singleton::getInstance()->mousey && 505 * SCREEN_HEIGHT / 1080 <Singleton::getInstance()->mousey))
	{
		//MOUSE CLICK
		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 30, 1, 0, 0, 0, false);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[33], Color(1, 0, 0), 1, 38, 30);
			Application::HideCursor();
			Application::SetMousePosition(0, 0);
			Singleton::getInstance()->pause = false;
		}
		//MOUSE HOVER
		else
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 30, 1, 0, 0, 0, false);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[33], Color(1, 0, 0), 1, 38, 30);
		}
	}
	//DEFAULT
	else
	{
		RenderUI(meshList[GEO_PAUSE_BUTTONS], 1, 40, 30, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[33], Color(1, 1, 1), 1, 38, 30);
	}

	////////////////////////////
	//     RESTART BUTTON     //
	////////////////////////////
	if ((1152 * SCREEN_WIDTH / 1920 > Singleton::getInstance()->mousex && 767 * SCREEN_WIDTH / 1920 < Singleton::getInstance()->mousex) &&
		(682 * SCREEN_HEIGHT / 1080 > Singleton::getInstance()->mousey && 612 * SCREEN_HEIGHT / 1080 <Singleton::getInstance()->mousey))
	{
		//MOUSE CLICK
		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			sound.stopMusic("Music//scenario3.mp3");
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 24, 1, 0, 0, 0, false);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[34], Color(1, 0, 0), 1, 37.5, 24);
			Singleton::getInstance()->stateCheck = true;
			Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME3;
			Object::objectMap.clear();
		}
		//MOUSE HOVER
		else
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 24, 1, 0, 0, 0, false);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[34], Color(1, 0, 0), 1, 37.5, 24);
		}
	}
	//DEFAULT
	else
	{
		RenderUI(meshList[GEO_PAUSE_BUTTONS], 1, 40, 24, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[34], Color(1, 1, 1), 1, 37.5, 24);
	}
	/////////////////////////
	//     EXIT BUTTON     //
	/////////////////////////
	if ((1152 * SCREEN_WIDTH / 1920 > Singleton::getInstance()->mousex && 767 * SCREEN_WIDTH / 1920 < Singleton::getInstance()->mousex) &&
		(789 * SCREEN_HEIGHT / 1080 > Singleton::getInstance()->mousey && 719 * SCREEN_HEIGHT / 1080 <Singleton::getInstance()->mousey))
	{
		//MOUSE CLICK
		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 18, 1, 0, 0, 0, false);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[36], Color(1, 0, 0), 1, 38.5, 18);
			Object::objectMap.clear();
			Singleton::getInstance()->program_state = Singleton::PROGRAM_EXIT;
		}
		//MOUSE HOVER
		else
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 18, 1, 0, 0, 0, false);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[36], Color(1, 0, 0), 1, 38.5, 18);
		}
	}
	//DEFAULT
	else
	{
		RenderUI(meshList[GEO_PAUSE_BUTTONS], 1, 40, 18, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[36], Color(1, 1, 1), 1, 38.5, 18);
	}
}
void SP2Scene3::win()
{
	RenderUI(meshList[GEO_WIN_BG], 5, 40, 35, 1.3, 0, 0, 0, false);

	/////////////////////////////
	//     MAINMENU BUTTON     //
	/////////////////////////////
	if ((1152 * SCREEN_WIDTH / 1920 > Singleton::getInstance()->mousex && 767 * SCREEN_WIDTH / 1920 < Singleton::getInstance()->mousex) &&
		(468 * SCREEN_HEIGHT / 1080 > Singleton::getInstance()->mousey && 398 * SCREEN_HEIGHT / 1080 <Singleton::getInstance()->mousey))
	{
		//MOUSE CLICK
		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 36, 1, 0, 0, 0, false);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[35], Color(1, 0, 0), 1, 37, 36);
			Singleton::getInstance()->stateCheck = true;
			Singleton::getInstance()->program_state = Singleton::PROGRAM_MENU;
			Object::objectMap.clear();
		}
		//MOUSE HOVER
		else
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 36, 1, 0, 0, 0, false);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[35], Color(1, 0, 0), 1, 37, 36);
		}
	}
	//DEFAULT
	else
	{
		RenderUI(meshList[GEO_PAUSE_BUTTONS], 1, 40, 36, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[35], Color(1, 1, 1), 1, 37, 36);
	}
	/////////////////////////
	//     EXIT BUTTON     //
	/////////////////////////
	if ((1152 * SCREEN_WIDTH / 1920 > Singleton::getInstance()->mousex && 767 * SCREEN_WIDTH / 1920 < Singleton::getInstance()->mousex) &&
		(575 * SCREEN_HEIGHT / 1080 > Singleton::getInstance()->mousey && 505 * SCREEN_HEIGHT / 1080 <Singleton::getInstance()->mousey))
	{
		//MOUSE CLICK
		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 30, 1, 0, 0, 0, false);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[36], Color(1, 0, 0), 1, 38.5, 30);
			Object::objectMap.clear();
			Singleton::getInstance()->program_state = Singleton::PROGRAM_EXIT;
		}
		//MOUSE HOVER
		else
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 30, 1, 0, 0, 0, false);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[36], Color(1, 0, 0), 1, 38.5, 30);
		}
	}
	//DEFAULT
	else
	{
		RenderUI(meshList[GEO_PAUSE_BUTTONS], 1, 40, 30, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[36], Color(1, 1, 1), 1, 38.5, 30);
	}
}
/******************************************************************************/
/*!
\brief
Exits the scene and delete

\exception None
\return None
*/
/******************************************************************************/
void SP2Scene3::Exit()
{
    glDeleteVertexArrays(1, &m_vertexArrayID);
    glDeleteProgram(m_programID);
}