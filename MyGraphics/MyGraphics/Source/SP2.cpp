#include "SP2.h"
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
SP2::SP2()
{
}

SP2::~SP2()
{
}

float distanceBetween(Vector3 from, Vector3 to){
    return sqrt((pow(to.x - from.x, 2)) + (pow(to.z - from.z, 2)));
}

void SP2::Init()
{
	// Init VBO here
	//sound.playMusic("Music//Music.mp3");
	Application::HideCursor();
	Singleton::getInstance()->pause = false;
	Singleton::getInstance()->buttonText = false;
    spawnRadius = 5000;
    oreFrequency = 100;
    treeFrequency = 100;
	heldDelay = 0;

    srand(time(0));
    planeInit();

	if (Singleton::getInstance()->program_state == Singleton::PROGRAM_GAME2R)
	{
		message = 32;
		mission = 5;
	}
	else
	{
		message = 0;
		mission = 0;
	}
	
	inputDelay = 0.0f;
	weaponDelay = 10.f;
	startingPlane.planePos = Vector3(0, 0, 0);
	startingPlane.planeMin = Vector3(0, 0, 0);
	startingPlane.planeMax = Vector3(300, 0, 300);
	swordPos = Vector3(0, 10, rand() % 30 + 1985);
	gunPos = Vector3(rand() % 30 + 1985, 1, 0);
	shipPos = Vector3((rand() % 2000) - 1000, 17, (rand() % 2000) - 1000);
	shipSize = Vector3(30, 30, 30);

	if (Singleton::getInstance()->stateCheck == Singleton::PROGRAM_GAME2R)
	{
		message = 32;
		mission = 5;
	}

	Dialogue("Text//NPC.txt");

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
	camera.Init(Vector3(0, 15, 0), Vector3(90, 0, 0), Vector3(0, 1, 0));
	Application::SetMousePosition(0, 0);

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("AXES", 500, 500, 500);

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("LIGHTBALL", Color(1, 1, 1), 10, 20);

	meshList[GEO_CYLIN] = MeshBuilder::GenerateCylin("FIST", Color(0.3f, 0.3f, 0.3f), 36);
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("FIST", Color(1.f, 0.863f, 0.698f), 36);

	meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("GROUND", Color(0.3f, 0.3f, 0.3f), TexCoord(10, 10), 1, 1);
	meshList[GEO_GROUND]->textureID = LoadTGA("Image//planet1_land.tga");

	meshList[GEO_ORE] = MeshBuilder::GenerateOBJ("ORE", "OBJ//Ore.obj");
	meshList[GEO_ORE]->textureID = LoadTGA("Image//TinOre.tga");

	meshList[GEO_PELICAN] = MeshBuilder::GenerateOBJ("GUN", "OBJ//air.obj");
	meshList[GEO_PELICAN]->textureID = LoadTGA("Image//air_UV.tga");

	meshList[GEO_IMAGES] = MeshBuilder::GenerateQuad("images", Color(1, 1, 1), TexCoord(1, 1), 1, 1);
	meshList[GEO_IMAGES]->textureID = LoadTGA("Image//images.tga");

	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateQuad("crosshair", Color(1, 1, 1), TexCoord(1, 1), 1, 1);
	meshList[GEO_CROSSHAIR]->textureID = LoadTGA("Image//crosshair.tga");

	meshList[GEO_HP_BAR_LOW] = MeshBuilder::GenerateOBJ("HEALTH", "OBJ//hp.obj");
	meshList[GEO_HP_BAR_LOW]->textureID = LoadTGA("Image//hp.tga");

	meshList[GEO_HP_BAR_MID] = MeshBuilder::GenerateOBJ("HEALTH", "OBJ//hp.obj");
	meshList[GEO_HP_BAR_MID]->textureID = LoadTGA("Image//hp_mid.tga");

	meshList[GEO_HP_BAR_HIGH] = MeshBuilder::GenerateOBJ("HEALTH", "OBJ//hp.obj");
	meshList[GEO_HP_BAR_HIGH]->textureID = LoadTGA("Image//hp_high.tga");

	meshList[GEO_MESSAGEBOX] = MeshBuilder::GenerateQuad("MESSAGEBOX", Color(1, 1, 1), TexCoord(1, 1), 6, 3);
	meshList[GEO_MESSAGEBOX]->textureID = LoadTGA("Image//messageBox.tga");

	meshList[GEO_QUESTLIST] = MeshBuilder::GenerateQuad("QUESTLIST", Color(1, 1, 1), TexCoord(1, 1), 3, 3);
	meshList[GEO_QUESTLIST]->textureID = LoadTGA("Image//questList.tga");

	meshList[GEO_STATS] = MeshBuilder::GenerateQuad("STATS", Color(1, 1, 1), TexCoord(1, 1), 3, 3);
	meshList[GEO_STATS]->textureID = LoadTGA("Image//background3.tga");

	meshList[GEO_PAUSE_BG] = MeshBuilder::GenerateQuad("PauseUI", Color(1, 1, 1), TexCoord(1, 1), 3, 4);
	meshList[GEO_PAUSE_BG]->textureID = LoadTGA("Image//background4.tga");

	meshList[GEO_PAUSE_BUTTONS] = MeshBuilder::GenerateQuad("pauseBotton", Color(1, 1, 1), TexCoord(1, 1), 8, 2);
	meshList[GEO_PAUSE_BUTTONS]->textureID = LoadTGA("Image//buttonDefault.tga");

	meshList[GEO_PAUSE_BUTTONS_HOVER] = MeshBuilder::GenerateQuad("pauseHover", Color(1, 1, 1), TexCoord(1, 1), 8, 2);
	meshList[GEO_PAUSE_BUTTONS_HOVER]->textureID = LoadTGA("Image//buttonHover.tga");

	meshList[GEO_SHIPDISTANCETAB] = MeshBuilder::GenerateQuad("SHIPDISTANCE", Color(1, 1, 1), TexCoord(1, 1), 2, 1);
	meshList[GEO_SHIPDISTANCETAB]->textureID = LoadTGA("Image//shipDistanceTab.tga");

	meshList[GEO_BORDER] = MeshBuilder::GenerateOBJ("HEALTH", "OBJ//hp.obj");
	meshList[GEO_BORDER]->textureID = LoadTGA("Image//border.tga");

	meshList[GEO_MINING_BAR] = MeshBuilder::GenerateOBJ("ORE", "OBJ//hp.obj");
	meshList[GEO_MINING_BAR]->textureID = LoadTGA("Image//miningBar.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("FRONT", Color(0, 0, 0), TexCoord(1, 1), 1, 1);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//planet1_ft.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("BACK", Color(0, 0, 0), TexCoord(1, 1), 1, 1);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//planet1_bk.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("TOP", Color(0, 0, 0), TexCoord(1, 1), 1, 1);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//planet1_up.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("BOTTOM", Color(0, 0, 0), TexCoord(1, 1), 1, 1);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//planet1_dn.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("RIGHT", Color(0, 0, 0), TexCoord(1, 1), 1, 1);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//planet1_rt.tga");
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("LEFT", Color(0, 0, 0), TexCoord(1, 1), 1, 1);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//planet1_lf.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("TEXT", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//font.tga");

    meshList[GEO_NPC1] = MeshBuilder::GenerateOBJ("NPC 1", "OBJ//NPC1_MAIN.obj");
    meshList[GEO_NPC1]->textureID = LoadTGA("Image//NPC.tga");
    meshList[GEO_NPC1_HAND1] = MeshBuilder::GenerateOBJ("NPC 1", "OBJ//NPC1_HAND1.obj");
    meshList[GEO_NPC1_HAND1]->textureID = LoadTGA("Image//NPC.tga");
    meshList[GEO_NPC1_HAND2] = MeshBuilder::GenerateOBJ("NPC 1", "OBJ//NPC1_HAND2.obj");
    meshList[GEO_NPC1_HAND2]->textureID = LoadTGA("Image//NPC.tga");
    meshList[GEO_NPC1_LEG1] = MeshBuilder::GenerateOBJ("NPC 1", "OBJ//NPC1_LEG1.obj");
    meshList[GEO_NPC1_LEG1]->textureID = LoadTGA("Image//NPC.tga");
    meshList[GEO_NPC1_LEG2] = MeshBuilder::GenerateOBJ("NPC 1", "OBJ//NPC1_LEG2.obj");
    meshList[GEO_NPC1_LEG2]->textureID = LoadTGA("Image//NPC.tga");

	meshList[GEO_NPC1_ICON] = MeshBuilder::GenerateQuad("NPC1_icon", Color(1, 1, 1), TexCoord(1, 1), 1, 1);
	meshList[GEO_NPC1_ICON]->textureID = LoadTGA("Image//NPCicon1.tga");

	meshList[GEO_QUEST] = MeshBuilder::GenerateOBJ("quest", "OBJ//quest.obj");
	meshList[GEO_QUEST]->textureID = LoadTGA("Image//quest.tga");
	meshList[GEO_QUEST]->material.kAmbient.Set(1.0f, 1.0f, 0.0f);
	meshList[GEO_QUEST]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_QUEST]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_QUEST]->material.kShininess = 1.f;

	meshList[GEO_HELPER] = MeshBuilder::GenerateOBJ("helper", "OBJ//helper.obj");
	meshList[GEO_HELPER]->textureID = LoadTGA("Image//helper.tga");
	meshList[GEO_HELPER]->material.kAmbient.Set(1.0f, 1.0f, 0.0f);
	meshList[GEO_HELPER]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_HELPER]->material.kSpecular.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_HELPER]->material.kShininess = 1.f;

	meshList[GEO_SWORD] = MeshBuilder::GenerateOBJ("SWORD", "OBJ//sword.obj");
	meshList[GEO_SWORD]->textureID = LoadTGA("Image//sword.tga");

	meshList[GEO_GUN] = MeshBuilder::GenerateOBJ("GUN", "OBJ//gun3.obj");
	meshList[GEO_GUN]->textureID = LoadTGA("Image//gun3.tga");

    meshList[GEO_TREE] = MeshBuilder::GenerateOBJ("TREE", "OBJ//tree.obj");
    meshList[GEO_TREE]->textureID = LoadTGA("Image//tree.tga");

    meshList[GEO_TENT] = MeshBuilder::GenerateOBJ("TENT", "OBJ//tent.obj");
    meshList[GEO_TENT]->textureID = LoadTGA("Image//house.tga");

	meshList[GEO_HITBOX] = MeshBuilder::GenerateCube("HITBOX", Color(1, 0, 0));

	for (auto q : orePos)
	{
		ore = new Object(Vector3(q.x, 5, q.z), Vector3(10, 20, 10), true);
	}

    for (auto q : treePos)
    {
        tree = new Object(Vector3(q.x, 5, q.z), Vector3(40, 100, 40), true);
    }
	
	ship = new Object(Vector3(shipPos.x, 15, shipPos.z), Vector3(shipSize.x * 6, shipSize.y * 2, shipSize.z * 4));
	sword = new Object(Vector3(swordPos.x, swordPos.y, swordPos.z), Vector3(7, 20, 7));
	gun = new Object(Vector3(gunPos.x, gunPos.y, gunPos.z), Vector3(7, 20, 7));
	ground = new Object(Vector3(camera.position.x, 7, camera.position.z), Vector3(500, 10, 500));

	melee = new Weapon(5 + (Singleton::getInstance()->oreCount * 2));
	ranged = new Weapon(3 + Singleton::getInstance()->oreCount);
	fist = new Weapon(1);
}

void SP2::Update(double dt)
{
	planeDistance = sqrtf((shipPos.x - camera.position.x) * (shipPos.x - camera.position.x) + (shipPos.y - camera.position.y) * (shipPos.y - camera.position.y) + (shipPos.z - camera.position.z) * (shipPos.z - camera.position.z));

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

		NPC2.setPos(NPC2.position);

		//interaction with NPC
		if (distanceBetween(NPC2.position, camera.position) < 15)
		{
			NPCshowPressE = true;
			if (Application::IsKeyPressed('E') && inputDelay >= 1)
			{
				//NPC dialogue start
				if (mission < 3)
				{
					mission = 1;
					message++;
					inputDelay = 0;

					if (message > 7)
					{
						message = 0;
						inputDelay = 0;
					}
				}

				//NPC dialogue after ship repair
				if (mission == 3)
				{
					message--;
					inputDelay = 0;

					if (message < -4)
					{
						mission = 4;
						message = 0;
						inputDelay = 0;
					}
				}

			}
		}
		//interaction with ship
		else if (planeDistance < 100 && mission != 4 && mission != 6)
		{
			//message start from 8
			if (!shipStatus && message == 0)
				message = 8;
			if (Application::IsKeyPressed('E') && inputDelay >= 1)
			{
				if (message == 8)
				{
					//if there's enough ore
					if (Singleton::getInstance()->objectCount[ore] >= 2)
					{
						message = 10;
						inputDelay = 0;
						Singleton::getInstance()->objectCount[ore] -= 2;
						mission = 2;
						shipStatus = true;
					}
					else
					{
						message = 9;
						inputDelay = 0;
					}
				}
				//after the mission is done
				if (mission == 2 && inputDelay >= 1 && message != 0)
				{
					message++;
					inputDelay = 0;

					if (message > 11)
					{
						message = 0;
						inputDelay = 0;
					}
				}
			}
		}

		//exiting the ship after mission is done
		else if (mission == 2 && planeDistance > 100)
		{
			message = 12;

			//exit the dialogue
			if (Application::IsKeyPressed('E') && message == 12 && inputDelay >= 1)
			{
				message = 0;
				inputDelay = 0;
				mission = 3;
			}
		}
		else if (mission == 4 || mission == 6)
		{
			if (planeDistance < 100)
			{
				message = 17;

				if (Application::IsKeyPressed('E'))
				{
					Singleton::getInstance()->oreCount += Singleton::getInstance()->objectCount[ore];
					Singleton::getInstance()->stateCheck = true;
					Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME2;
					Object::objectMap.clear();
				}
			}
			if (distanceBetween(NPC2.position, camera.position) >= 30)
			{
				if (NPC2.position.x <= camera.position.x + 40)
					NPC2.position.x += (float)(150 * dt);

				if (NPC2.position.x >= camera.position.x - 40)
					NPC2.position.x -= (float)(150 * dt);

				if (NPC2.position.z <= camera.position.z + 40)
					NPC2.position.z += (float)(150 * dt);

				if (NPC2.position.z >= camera.position.z - 40)
					NPC2.position.z -= (float)(150 * dt);
			}
		}
		else if (mission == 5)
		{
			if (Application::IsKeyPressed('E') && message == 32 && inputDelay >= 1)
			{
				message = 0;
				mission = 6;
			}

		}

		else
		{
			message = 0;
			NPCshowPressE = false;
		}

		for (auto q : Singleton::getInstance()->objectCount)
		{
			cout << q.second << endl;
		}
		if (Application::IsKeyPressed('1')) //enable back face culling
			glEnable(GL_CULL_FACE);
		if (Application::IsKeyPressed('2')) //disable back face culling
			glDisable(GL_CULL_FACE);
		if (Application::IsKeyPressed('3'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
		if (Application::IsKeyPressed('4'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
		if (Application::IsKeyPressed(VK_TAB) && inputDelay >= 1)
		{
			if (!pressTab)
			{
				pressTab = true;
				inputDelay = 0;
			}
			else
			{
				pressTab = false;
				inputDelay = 0;
			}
		}
		if (pressTab)
		{
			questTab += (float)(40 * dt);

			if (questTab >= 91.8)
				questTab = 91.8;
		}

		if (!pressTab)
		{
			questTab -= (float)(40 * dt);

			if (questTab <= 65)
				questTab = 65;
		}
		if (showDistance)
		{
			shipTab -= (float)(10 * dt);

			if (shipTab <= 60)
				shipTab = 60;
		}

		if (!showDistance)
		{
			shipTab += (float)(10 * dt);

			if (shipTab >= 65)
				shipTab = 65;
		}
		if (Application::IsKeyPressed(VK_LBUTTON) && !Singleton::getInstance()->swordAniDown && !Singleton::getInstance()->swordAniUp && Singleton::getInstance()->gotSword)
		{
			Singleton::getInstance()->swordAniDown = true;
		}
		if (Singleton::getInstance()->swordAniDown)
		{
			Singleton::getInstance()->rotateSword += (float)(500 * dt);
			if (Singleton::getInstance()->rotateSword >= 120)
			{
				Singleton::getInstance()->swordAniDown = false;
				Singleton::getInstance()->swordAniUp = true;
			}
		}
		if (Singleton::getInstance()->swordAniUp)
		{
			Singleton::getInstance()->rotateSword -= (float)(500 * dt);
			if (Singleton::getInstance()->rotateSword <= 20)
			{
				Singleton::getInstance()->swordAniDown = false;
				Singleton::getInstance()->swordAniUp = false;
			}
		}
        for (auto q : Object::objectMap)
		{
            for (vector<Bullet*>::iterator it = Bullet::bulletVec.begin(); it != Bullet::bulletVec.end();)
            {
                if (!(q.first->hitbox.isTouching((*it)->pos))){
                    (*it)->pos += (*it)->dir * (*it)->speed * dt;
                }
                if (distanceBetween((*it)->pos, camera.position) > 500){
                    delete *it;
                    it = Bullet::bulletVec.erase(it);
                }
                else{
                    it++;
                }
            }
        }
        if (Application::IsKeyPressed(VK_RBUTTON) && !Singleton::getInstance()->gunAniDown && !Singleton::getInstance()->gunAniUp && Singleton::getInstance()->gotGun)
        {
			bullet = new Bullet(Vector3(camera.target), Vector3(1, 1, 1), Vector3(camera.view), 25);
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

		if (Application::IsKeyPressed('P'))
		{
			Singleton::getInstance()->pause = true;
			Application::ShowCursor();
		}

		if (Application::IsKeyPressed('K'))
		{
			Singleton::getInstance()->health -= 5;

			if (Singleton::getInstance()->health <= 50) hpMid = true;
			if (Singleton::getInstance()->health <= 25) hpLow = true;
			if (Singleton::getInstance()->health <= 0) Singleton::getInstance()->health = 0;
		}

		if (Application::IsKeyPressed('L'))
		{
			Singleton::getInstance()->health += 5;

			if (Singleton::getInstance()->health >= 50) hpMid = false;
			if (Singleton::getInstance()->health >= 25) hpLow = false;
			if (Singleton::getInstance()->health >= 100) Singleton::getInstance()->health = 100;
		}
        if (Application::IsKeyPressed('R'))
        {
			Object::objectMap.clear();
			Singleton::getInstance()->stateCheck = true;
			Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME;
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

		planeLoader();

		for (auto q : Object::objectMap)
		{
			for (int i = 0; i < orePos.size(); ++i)
			{
				if (orePos[i].x == q.first->pos.x && orePos[i].z == q.first->pos.z)
				{
					if (Application::IsKeyPressed('E') && q.first->hitbox.isTouching(camera.target))
					{
						heldDelay += 1.f * dt;
						if (heldDelay > 2)
						{
							Singleton::getInstance()->handDown = true;
							Singleton::getInstance()->fistDown = true;
							Inventory::addObject(ore);
							heldDelay = 0;
							delete q.first;
						}
					}
				}
				miningDisplay = true;
			}
		}

		if (Application::IsKeyPressed('E') && sword->hitbox.isTouching(camera.target))
		{
			swordPos.y += 1 * dt;
			pickSword += 1 * dt;
			if (pickSword > 5)
			{
				pickSword = 0;
				Inventory::addObject(sword);
				Singleton::getInstance()->gotSword = true;
				delete sword;
			}
		}

		if (gun->hitbox.isTouching(camera.target))
		{
			Inventory::addObject(gun);
			Singleton::getInstance()->gotGun = true;
			delete gun;
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
		if (weaponDelay <= 10)
		{
			weaponDelay += 20 * dt;
		}
		camera.Update(dt);
	}
}

void SP2::Dialogue(string filename)
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

void SP2::RenderMesh(Mesh *mesh, bool enableLight)
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

void SP2::Render()
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
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
	RenderSkybox();
	modelStack.PopMatrix();
	//t->r->s
	//RenderMesh(meshList[GEO_AXES], false);

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	//RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	// hitbox outline
	for (auto q : Object::objectMap)
	{
		modelStack.PushMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		modelStack.Translate(q.first->pos.x, q.first->pos.y, q.first->pos.z);
		modelStack.Scale(q.first->size.x, q.first->size.y, q.first->size.z);
		//RenderMesh(meshList[GEO_HITBOX], false);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		modelStack.PopMatrix();
	}

	for (auto q : Bullet::bulletVec)
	{
		modelStack.PushMatrix();
		modelStack.Translate(q->pos.x, q->pos.y, q->pos.z);
		RenderMesh(meshList[GEO_LIGHTBALL], false);
		modelStack.PopMatrix();
	}

    modelStack.PushMatrix();
	modelStack.Scale(10, 10, 10);
    RenderMesh(meshList[GEO_TENT], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(NPC2.position.x, NPC2.position.y, NPC2.position.z);
	modelStack.Translate(0, -8, 0);
	modelStack.Scale(4, 4, 4);

	modelStack.PushMatrix();
	//modelStack.Rotate(angleBetween(boss.position, camera.position), 0, 1, 0);
	RenderMesh(meshList[GEO_NPC1], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_NPC1_HAND1], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_NPC1_HAND2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_NPC1_LEG1], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_NPC1_LEG2], true);
	modelStack.PopMatrix();

    modelStack.PopMatrix();

	for (auto q : Object::objectMap)
	{
		if (q.first->pos.x == swordPos.x && q.first->pos.z == swordPos.z && !Singleton::getInstance()->gotSword)
		{
			modelStack.PushMatrix();
			modelStack.Translate(swordPos.x, swordPos.y + 5, swordPos.z);
			modelStack.Rotate(180, 1, 0, 0);
			modelStack.Scale(3, 3, 3);
			RenderMesh(meshList[GEO_SWORD], true);
			modelStack.PopMatrix();
		}
	}
	for (auto q : Object::objectMap)
	{
		if (q.first == gun && !Singleton::getInstance()->gotGun && !Singleton::getInstance()->gotGun)
		{
			modelStack.PushMatrix();
			modelStack.Translate(gunPos.x, gunPos.y, gunPos.z);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(10, 10, 10);
			RenderMesh(meshList[GEO_GUN], true);
			modelStack.PopMatrix();
		}
	}

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

	for (auto q : Object::objectMap)
	{
        //Ore Render
		for (int j = 0; j < orePos.size(); ++j)
		{
			if ((q.first->pos.x == orePos[j].x) &&
				(q.first->pos.z == orePos[j].z))
			{
                if (orePos[j].x >= camera.position.x - 80 || orePos[j].x <= camera.position.x + 80 || orePos[j].z >= camera.position.z - 80 || orePos[j].z <= camera.position.z + 80){
                    modelStack.PushMatrix();
                    modelStack.Translate(orePos[j].x, 0, orePos[j].z);
                    modelStack.Scale(4, 4, 4);
                    RenderMesh(meshList[GEO_ORE], true);
                    modelStack.PopMatrix();
                }
			}
		}
        //Tree Render
        for (int j = 0; j < treePos.size(); ++j)
        {
            if ((q.first->pos.x == treePos[j].x) &&
                (q.first->pos.z == treePos[j].z))
            {
                if (treePos[j].x >= camera.position.x - 80 || treePos[j].x <= camera.position.x + 80 || treePos[j].z >= camera.position.z - 80 || treePos[j].z <= camera.position.z + 80){
                    modelStack.PushMatrix();
                    modelStack.Translate(treePos[j].x, 0, treePos[j].z);
                    modelStack.Scale(8, 8, 8);
                    RenderMesh(meshList[GEO_TREE], true);
                    modelStack.PopMatrix();

                }
            }
        }
	}

	modelStack.PushMatrix();
	modelStack.Translate(
		camera.target.x,
		camera.target.y,
		camera.target.z
		);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	//RenderMesh(meshList[GEO_LIGHTBALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(shipPos.x, shipPos.y, shipPos.z);
	modelStack.Scale(shipSize.x, shipSize.y, shipSize.z);
	RenderMesh(meshList[GEO_PELICAN], true);
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

	if (miningDisplay && heldDelay >= 0.1)
	{
		RenderUI(meshList[GEO_MINING_BAR], 2, 29, 15, heldDelay * 5, 0, 0, 0, false);
		RenderUI(meshList[GEO_BORDER], 2, 29, 15, 10, 0, 0, 0, false);
	}

	RenderUI(meshList[GEO_QUESTLIST], 5, questTab, 40, 1, 0, 0, 0, false);
	RenderUI(meshList[GEO_SHIPDISTANCETAB], 5, 40, shipTab, 2, 0, 0, 0, false);
	RenderTextOnScreen(meshList[GEO_TEXT], "SHIP DISTANCE : " + std::to_string(planeDistance), Color(1, 1, 1), 1.2, 30, shipTab - 2.5);

	if (questText == ". Repair Ship")
	{
		RenderUI(meshList[GEO_ORE], 2, questTab - 5, 37, 1, 20, roateQuest, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], "Ores: " + std::to_string(Singleton::getInstance()->objectCount[ore]) + " / 2", Color(1, 1, 0), 1, questTab, 39);
	}

	if (!hpMid && !hpLow)
		RenderUI(meshList[GEO_HP_BAR_HIGH], 2, 10, 10, Singleton::getInstance()->health / 10, 0, 0, 0, false);

	if (hpMid && !hpLow)
		RenderUI(meshList[GEO_HP_BAR_MID], 2, 10, 10, Singleton::getInstance()->health / 10, 0, 0, 0, false);

	if (hpMid && hpLow)
		RenderUI(meshList[GEO_HP_BAR_LOW], 2, 10, 10, Singleton::getInstance()->health / 10, 0, 0, 0, false);

	RenderUI(meshList[GEO_BORDER], 2, 10, 10, 10, 0, 0, 0, false);
	RenderTextOnScreen(meshList[GEO_TEXT], "HP: ", Color(0, 1, 0), 2, 5, 10);

	RenderUI(meshList[GEO_CROSSHAIR], 1, 40, 30, 1, 0, 0, 0, false);
	if (NPCshowPressE && mission != 4)
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[25], Color(1, 1, 0), 1.5, 25, 15);
	if (OREshowPressE)
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[26], Color(1, 1, 0), 1.5, 25, 15);

	int j = 14;
	switch (message)
	{
	
	case 1://Ah, you've finally woken up!
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		RenderUI(meshList[GEO_NPC1_ICON], 5, 13, 10, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[0], Color(1, 1, 0), 1.5, 23, 10);
		break;
	case 2://Your ship has crashed but fret not!
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		RenderUI(meshList[GEO_NPC1_ICON], 5, 13, 10, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[1], Color(1, 1, 0), 1.5, 23, 10);
		break;
	case 3://My home planet is full of tin ore for you to use to repair your ship!
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		RenderUI(meshList[GEO_NPC1_ICON], 5, 13, 10, 1, 0, 0, 0, false);

		for (int i = 2; i <= 3; ++i)
		{
			j -= 2;
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[i], Color(1, 1, 0), 1.5, 23, j);
		}
		break;
	case 4://Of course we would have to locate it too.
		showDistance = true;
		questText = ". Find ship";
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		RenderUI(meshList[GEO_NPC1_ICON], 5, 13, 10, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[4], Color(1, 1, 0), 1.3, 23, 10);
		break;
	case 5://Legend speaks of a sword that resides from where the sun rises. 
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		RenderUI(meshList[GEO_NPC1_ICON], 5, 13, 10, 1, 0, 0, 0, false);

		for (int i = 5; i <= 6; ++i)
		{
			j -= 2;
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[i], Color(1, 1, 0), 1.5, 23, j);
		}
		break;
	case 6://Perhaps that might be of use to you in your adventures.
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		RenderUI(meshList[GEO_NPC1_ICON], 5, 13, 10, 1, 0, 0, 0, false);

		for (int i = 7; i <= 8; ++i)
		{
			j -= 2;
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[i], Color(1, 1, 0), 1.5, 23, j);
		}
		break;
	case 7://If it helps, my house faces the north.
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		RenderUI(meshList[GEO_NPC1_ICON], 5, 13, 10, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[9], Color(1, 1, 0), 1.5, 23, 10);
		break;
	case 8://Would you like to repair your ship? Cost: 2 Ores
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		//RenderUI(meshList[GEO_NPC1_ICON], 5, 13, 10, 1, 0, 0, 0, false);

		for (int i = 10; i <= 11; ++i)
		{
			j -= 2;
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[i], Color(1, 1, 0), 1.5, 23, j);
		}
		break;
	case 9://You need more ores.
		questText = ". Repair Ship";
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		//RenderUI(meshList[GEO_NPC1_ICON], 5, 13, 10, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[12], Color(1, 1, 0), 1.5, 23, 10);
		break;
	case 10://Repair successful
		questText = "";
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		//RenderUI(meshList[GEO_NPC1_ICON], 5, 13, 10, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[13], Color(1, 1, 0), 1.5, 23, 10);
		break;
	case 11://Now that your ship has been repaired,perhaps you might want to gather more ore so that your ship would fare better out there in space?
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		//RenderUI(meshList[GEO_NPC1_ICON], 5, 13, 10, 1, 0, 0, 0, false);

		for (int i = 14; i <= 17; ++i)
		{
			j -= 2;
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[i], Color(1, 1, 0), 1.5, 23, j);
		}
		break;
	case 12://I have words with you.
		questText = ". Go to NPC";
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		RenderUI(meshList[GEO_NPC1_ICON], 5, 13, 10, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[18], Color(1, 1, 0), 1.5, 23, 10);
		break;
	case -1://I need to ask you for a favour.
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		RenderUI(meshList[GEO_NPC1_ICON], 5, 13, 10, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[19], Color(1, 1, 0), 1.5, 23, 10);
		break;
	case -2://I need your help to get me to my home planet.
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		RenderUI(meshList[GEO_NPC1_ICON], 5, 13, 10, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[20], Color(1, 1, 0), 1.3, 23, 10);
		break;
	case -3://It's the neighbouring planet. 
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		RenderUI(meshList[GEO_NPC1_ICON], 5, 13, 10, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[21], Color(1, 1, 0), 1.5, 23, 10);
		break;
	case -4://I have no ship of my own to fly there.
		questText = ". Go to ship";
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		RenderUI(meshList[GEO_NPC1_ICON], 5, 13, 10, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[22], Color(1, 1, 0), 1.5, 23, 10);
		break;
	case 17://Do you want to travel to other planet?
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		//RenderUI(meshList[GEO_NPC1_ICON], 5, 13, 10, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[23], Color(1, 1, 0), 1.5, 23, 10);
		break;
	case 32://Ship crash
		RenderUI(meshList[GEO_MESSAGEBOX], 3.6, 40, 10, 1.8, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[24], Color(1, 1, 0), 1.5, 65, 3);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[31], Color(1, 1, 0), 1.5, 22, 13);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[32], Color(1, 1, 0), 1.5, 15, 8);
	default:
		break;
	}
	RenderTextOnScreen(meshList[GEO_TEXT], questText, Color(1, 1, 0), 1, questTab - 5, 45);
	RenderTextOnScreen(meshList[GEO_TEXT], FPS + " FPS", Color(0, 1, 0), 1, 1, 1);	// fps
	RenderTextOnScreen(meshList[GEO_TEXT], "POSITION X: " + std::to_string(camera.position.x), Color(0, 0, 0), 1, 1, 50);
	RenderTextOnScreen(meshList[GEO_TEXT], "POSITION Z: " + std::to_string(camera.position.z), Color(0, 0, 0), 1, 1, 48);
	RenderTextOnScreen(meshList[GEO_TEXT], "Mouse X: " + std::to_string(Singleton::getInstance()->mousex), Color(0, 0, 0), 1, 1, 46);
	RenderTextOnScreen(meshList[GEO_TEXT], "Mouse Z: " + std::to_string(Singleton::getInstance()->mousey), Color(0, 0, 0), 1, 1, 44);
	RenderTextOnScreen(meshList[GEO_TEXT], "Plane Distance : " + std::to_string(planeDistance), Color(0, 0, 0), 1, 1, 42);
	RenderTextOnScreen(meshList[GEO_TEXT], "Pause Check" + std::to_string(Singleton::getInstance()->pause), Color(0, 0, 0), 1, 1, 38);
	RenderTextOnScreen(meshList[GEO_TEXT], "MESSAGE CHECK : " + std::to_string(message), Color(0, 0, 0), 1, 1, 32);
	RenderTextOnScreen(meshList[GEO_TEXT], "MISSION CHECK : " + std::to_string(mission), Color(0, 0, 0), 1, 1, 30);
	
	for (auto q : Singleton::getInstance()->objectCount)
	{
		if (q.first == ore)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Ores: " + std::to_string(q.second + Singleton::getInstance()->oreCount), Color(0, 0, 0), 1, 1, 36);
		}
	}

	RenderTextOnScreen(meshList[GEO_TEXT], "Mouse Speed: " + std::to_string(toupper(Singleton::getInstance()->MOUSE_SPEED)), Color(0, 0, 0), 1, 1, 28);
	if (Singleton::getInstance()->buttonText == true)
		RenderTextOnScreen(meshList[GEO_TEXT], "Button Click", Color(0, 0, 0), 1, 40, 25);

	if (Application::IsKeyPressed('I'))
	{
		RenderUI(meshList[GEO_STATS], 5, 40, 30, 1.3, 0, 0, 0, false);

		RenderUI(meshList[GEO_SWORD], 1, 28, 30, 1, 0, roateQuest, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], "Melee damage: " + std::to_string(melee->getDamage() + (Singleton::getInstance()->objectCount[ore] * 2)), Color(1, 1, 1), 1, 35, 30);

		RenderUI(meshList[GEO_GUN], 10, 28, 25, 1, 0, roateQuest, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], "Ranged damage: " + std::to_string(ranged->getDamage() + Singleton::getInstance()->objectCount[ore]), Color(1, 1, 1), 1, 35, 25);
	}
	if (Singleton::getInstance()->pause == true)
		pause();
}

void SP2::RenderSkybox()
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

void SP2::RenderText(Mesh* mesh, std::string text, Color color)
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

void SP2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void SP2::RenderUI(Mesh* mesh, float size, float x, float y, float scaleX, float rotatex, float rotatey, float rotatez, bool enableLight)
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
	modelStack.Translate(x, y, 50);
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

void SP2::planeInit(bool reset){
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

        landMaxX = planeMap[2].planeMax.x;
        landMinX = planeMap[6].planeMin.x;
        landMaxZ = planeMap[2].planeMax.z;
        landMinZ = planeMap[6].planeMin.z;

        //Ore spawning Freq = 4
        for (int loop = 0; loop < oreFrequency / 4; loop++)
        {
            orePos.push_back(Vector3(rand() % spawnRadius, 0, rand() % spawnRadius));
        }
        for (int loop = 0; loop < oreFrequency / 4; loop++)
        {
            orePos.push_back(Vector3((rand() % spawnRadius) - spawnRadius, 0, rand() % spawnRadius));
        }
        for (int loop = 0; loop < oreFrequency / 4; loop++)
        {
            orePos.push_back(Vector3((rand() % spawnRadius) - spawnRadius, 0, (rand() % spawnRadius) - spawnRadius));
        }
        for (int loop = 0; loop < oreFrequency / 4; loop++)
        {
            orePos.push_back(Vector3(rand() % spawnRadius, 0, (rand() % spawnRadius) - spawnRadius));
        }

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

        landMaxX = planeMap[2].planeMax.x;
        landMinX = planeMap[6].planeMin.x;
        landMaxZ = planeMap[2].planeMax.z;
        landMinZ = planeMap[6].planeMin.z;
    }
}                                                              
                                                                          
void SP2::planeLoader(){

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
void SP2::pause()
{
	RenderUI(meshList[GEO_PAUSE_BG], 5, 40, 30, 1.3, 0, 0, 0, false);


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
		(700 * SCREEN_HEIGHT / 1080 > Singleton::getInstance()->mousey && 630 * SCREEN_HEIGHT / 1080 <Singleton::getInstance()->mousey))
	{
		//MOUSE CLICK
		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 23, 1, 0, 0, 0, false);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[34], Color(1, 0, 0), 1, 37.5, 23);
			Singleton::getInstance()->stateCheck = true;
			Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME;
			Object::objectMap.clear();
		}
		//MOUSE HOVER
		else
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 23, 1, 0, 0, 0, false);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[34], Color(1, 0, 0), 1, 37.5, 23);
		}
	}
	//DEFAULT
	else
	{
		RenderUI(meshList[GEO_PAUSE_BUTTONS], 1, 40, 23, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[34], Color(1, 1, 1), 1, 37.5, 23);
	}
	/////////////////////////
	//     EXIT BUTTON     //
	/////////////////////////
	if ((1152 * SCREEN_WIDTH / 1920 > Singleton::getInstance()->mousex && 767 * SCREEN_WIDTH / 1920 < Singleton::getInstance()->mousex) &&
		(830 * SCREEN_HEIGHT / 1080 > Singleton::getInstance()->mousey && 755 * SCREEN_HEIGHT / 1080 <Singleton::getInstance()->mousey))
	{
		//MOUSE CLICK
		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 16, 1, 0, 0, 0, false);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[35], Color(1, 0, 0), 1, 38.5, 16);
			Object::objectMap.clear();
			Singleton::getInstance()->program_state = Singleton::PROGRAM_EXIT;
		}
		//MOUSE HOVER
		else
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 16, 1, 0, 0, 0, false);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[35], Color(1, 0, 0), 1, 38.5, 16);
		}
	}
	//DEFAULT
	else
	{
		RenderUI(meshList[GEO_PAUSE_BUTTONS], 1, 40, 16, 1, 0, 0, 0, false);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[35], Color(1, 1, 1), 1, 38.5, 16);
	}
}

void SP2::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}