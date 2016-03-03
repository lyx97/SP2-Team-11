#include "SP2Scene2.h"
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

SP2Scene2::SP2Scene2()
{
}

SP2Scene2::~SP2Scene2()
{
}

void SP2Scene2::Init()
{
	// Init VBO here

	sound.playMusic("Music//Music.mp3");
	Application::HideCursor();
	Singleton::getInstance()->pause = false;
	Singleton::getInstance()->buttonText = false;
	pelicanPos = Vector3(-700, -3, 0);
	inputDelay = 9.0f;
	moonDistance = 0;
	handling = 10 + Singleton::getInstance()->oreCount;
	turningspeed = 10 + Singleton::getInstance()->oreCount * 2;
	repair = 10 + Singleton::getInstance()->oreCount;
	acceleration = 10 + Singleton::getInstance()->oreCount;
	hp = 10 * repair;

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
	camera.Init(Vector3(-700, 7, 0), Vector3(90, 0, 0), Vector3(0, 1, 0));

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("AXES", 500, 500, 500);

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("LIGHTBALL", Color(1, 1, 1), 10, 20);

	meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("GROUND", Color(0.3f, 0.3f, 0.3f), TexCoord(10, 10), 1, 1);
	meshList[GEO_GROUND]->textureID = LoadTGA("Image//planet1_land.tga");

	meshList[GEO_HOUSE2] = MeshBuilder::GenerateCube("WALLS", Color(0.3f, 0.3f, 0.3f));

	meshList[GEO_ORE] = MeshBuilder::GenerateOBJ("ORE", "OBJ//Ore.obj");
	meshList[GEO_ORE]->textureID = LoadTGA("Image//TinOre.tga");

	meshList[GEO_PELICAN] = MeshBuilder::GenerateOBJ("GUN", "OBJ//air.obj");
	meshList[GEO_PELICAN]->textureID = LoadTGA("Image//air_UV.tga");

	meshList[GEO_ROCK] = MeshBuilder::GenerateOBJ("ROCK", "OBJ//rock.obj");
	meshList[GEO_ROCK]->textureID = LoadTGA("Image//rock.tga");

	meshList[GEO_MOON] = MeshBuilder::GenerateOBJ("MOON", "OBJ//moon.obj");
	meshList[GEO_MOON]->textureID = LoadTGA("Image//moon2.tga");

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

	meshList[GEO_PAUSE_BG] = MeshBuilder::GenerateQuad("PauseUI", Color(1, 1, 1), TexCoord(1, 1), 3, 4);
	meshList[GEO_PAUSE_BG]->textureID = LoadTGA("Image//background4.tga");

	meshList[GEO_PAUSE_BUTTONS] = MeshBuilder::GenerateQuad("pauseBotton", Color(1, 1, 1), TexCoord(1, 1), 8, 2);
	meshList[GEO_PAUSE_BUTTONS]->textureID = LoadTGA("Image//buttonDefault.tga");

	meshList[GEO_PAUSE_BUTTONS_HOVER] = MeshBuilder::GenerateQuad("pauseHover", Color(1, 1, 1), TexCoord(1, 1), 8, 2);
	meshList[GEO_PAUSE_BUTTONS_HOVER]->textureID = LoadTGA("Image//buttonHover.tga");

	meshList[GEO_SHIPDISTANCETAB] = MeshBuilder::GenerateQuad("SHIPDISTANCE", Color(1, 1, 1), TexCoord(1, 1), 2, 1);
	meshList[GEO_SHIPDISTANCETAB]->textureID = LoadTGA("Image//shipDistanceTab.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("FRONT", Color(0, 0, 0), TexCoord(1, 1), 1, 1);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//stars_ft.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("BACK", Color(0, 0, 0), TexCoord(1, 1), 1, 1);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//stars_bk.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("TOP", Color(0, 0, 0), TexCoord(1, 1), 1, 1);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//stars_up.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("BOTTOM", Color(0, 0, 0), TexCoord(1, 1), 1, 1);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//stars_dn.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("RIGHT", Color(0, 0, 0), TexCoord(1, 1), 1, 1);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//stars_rt.tga");
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("LEFT", Color(0, 0, 0), TexCoord(1, 1), 1, 1);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//stars_lf.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("TEXT", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//font.tga");

	meshList[GEO_NPC1] = MeshBuilder::GenerateOBJ("NPC 1", "OBJ//NPC1_MAIN.obj");
	meshList[GEO_NPC1]->textureID = LoadTGA("Image//NPC.tga");

	meshList[GEO_HITBOX] = MeshBuilder::GenerateCube("HITBOX", Color(1, 0, 0));
	srand(time(0));
	for (int loop = 0; loop < rockfreq; loop++)
	{
		rockpos.push_back(Vector3((rand() % 1000)-500 , (rand() % 30) - 15, (rand() % 3000) - 1500));
	}

	for (int loop = 0; loop < 100; loop++)
	{
		rockpos.push_back(Vector3((rand() % 1000) - 500, (rand() % 600) - 300, (rand() % 3000) - 1500));
	}
	for (auto q : rockpos)
	{
		rock = new Object(Vector3(q.x, q.y, q.z), Vector3(60, 50, 70));
	}
    Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME2;
}

void SP2Scene2::Update(double dt)
{
	moonDistance = sqrtf((800 - pelicanPos.x) * (800 - pelicanPos.x) + (0 - pelicanPos.y) * (0 - pelicanPos.y) + (0 - pelicanPos.z) * (0 - pelicanPos.z));
	if (moonDistance < 400)
	{
		moonDistance = 400;
		Singleton::getInstance()->stateCheck = true;
		Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME3;
		Object::objectMap.clear();
	}
	if (death == true)
	{
		SP2Scene2::Reset();
	}
	
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
		if (Application::IsKeyPressed('1')) //enable back face culling
			glEnable(GL_CULL_FACE);
		if (Application::IsKeyPressed('2')) //disable back face culling
			glDisable(GL_CULL_FACE);
		if (Application::IsKeyPressed('3'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
		if (Application::IsKeyPressed('4'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

		if (Application::IsKeyPressed('P'))
		{
			Singleton::getInstance()->pause = true;
			Application::ShowCursor();
		}


		if (hp <= 50)
			hpMid = true;

		if (hp <= 25)
			hpLow = true;

		if (hp <= 0)
		{
			hp = 0;
			death = true;
		}
		if (hp >= 50)
			hpMid = false;

		if (hp >= 25)
			hpLow = false;

		if (hp >= 100)
			hp = 100;

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

		if (inputDelay <= 10.0f)
		{
			inputDelay += (float)(1 * dt);
		}
		else
		{
			camera.Update(dt);
		}

		if (Application::IsKeyPressed('W'))
		{
			if (momentum.x >= 3)
			{
				momentum.x = 3;
				cout << "max" << endl;
			}
			else if(momentum.x <= -3)
			{
				momentum.x = -3;
			}
			if (momentum.z >= 3)
			{
				momentum.z = 3;
			}
			else if (momentum.z <= -3)
			{
				momentum.z = -3;
			}
			
			if (rotation <= 90 && rotation >= 0)
			{
				momentum.x = momentum.x + (handling * dt / 10) + (acceleration*dt/5) * (1 - rotation / 90);
				pelicanPos.x += momentum.x;
				momentum.z = momentum.z - (handling * dt / 10) - (acceleration*dt / 5) * (rotation / 90);
				pelicanPos.z += momentum.z;

			}
			else if (rotation >= -90 && rotation <= 0)
			{
				momentum.x = momentum.x + (handling * dt / 10) + (acceleration*dt / 5) * (1 + (rotation / 90));
				pelicanPos.x += momentum.x;
				momentum.z = momentum.z + (handling * dt / 10) - (acceleration*dt / 5) * (rotation / 90);
				pelicanPos.z += momentum.z;

			}
			else if (rotation > 90)
			{
				momentum.x = momentum.x - (handling * dt / 10) - (acceleration*dt / 5) * ((rotation - 90) / 90);
				pelicanPos.x += momentum.x;
				momentum.z = momentum.z - (handling * dt / 10) - (acceleration*dt / 5) * (1 - (rotation - 90) / 90);
				pelicanPos.z += momentum.z;
			}
			else if (rotation < -90)
			{
				momentum.x = momentum.x - (handling * dt / 10) + (acceleration*dt / 5) * ((rotation + 90) / 90);
				pelicanPos.x += momentum.x;
				momentum.z = momentum.z + (handling * dt / 10) + (acceleration*dt / 5) * (1 + (rotation + 90) / 90);
				pelicanPos.z += momentum.z;
			}

		}
		if (momentum.x > 0.001)
		{
			momentum.x -= dt * handling / 10;
			if (!Application::IsKeyPressed('W'))
			{
				pelicanPos.x += momentum.x;
			}
		}
		else if (momentum.x < -0.001)
		{
			momentum.x += dt * handling / 10;
			if (!Application::IsKeyPressed('W'))
			{
				pelicanPos.x += momentum.x;
			}
		}
		else
		{
			momentum.x = 0;
		}


		if (momentum.z > 0.0001)
		{
			momentum.z -= dt * handling / 10;
			if (!Application::IsKeyPressed('W'))
			{
				pelicanPos.z += momentum.z;
			}
		}
		else if (momentum.z < -0.0001)
		{
			momentum.z += dt * handling / 10;
			if (!Application::IsKeyPressed('W'))
			{
				pelicanPos.z += momentum.z;
			}
		}
		else
		{
			momentum.z = 0;
		}

		if (rotation >= 180)
		{
			rotation = -180;
		}
		else if (rotation <= -180)
		{
			rotation = 180;
		}
		if (Application::IsKeyPressed('A'))
		{
			turning = turning + (dt * handling + dt * turningspeed) / 10;
			rotation = rotation + turning;
			if (turning > 2)
			{
				turning = 2;
			}
		}

		else if (Application::IsKeyPressed('D'))
		{
			turning = turning - (dt * handling + dt * turningspeed) / 10;
			rotation = rotation + turning;
			if (turning < -2)
			{
				turning = -2;
			}
		}

		if (turning >= 0.05)
		{
			turning = turning - dt * handling/10;
			if (!Application::IsKeyPressed('A') && !Application::IsKeyPressed('D'))
			{
				rotation = rotation + turning;
			}
		}

		else if (turning <= -0.05)
		{
			turning = turning + dt * handling/10;
			if (!Application::IsKeyPressed('A') && !Application::IsKeyPressed('D'))
			{
				rotation = rotation + turning;
			}
		}

		for (auto q : Object::objectMap)
		{
			if (q.first->hitbox.isTouching(pelicanPos))
			{
				hp -= 34;
				delete q.first;
			}
		}

		
		if (Application::IsKeyPressed('L'))
		{
			handling += 1;
		}
	}
}
void SP2Scene2::Dialogue(string filename)
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
void SP2Scene2::RenderMesh(Mesh *mesh, bool enableLight)
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

void SP2Scene2::Render()
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
	//modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
	RenderSkybox();
	modelStack.PopMatrix();
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

	modelStack.PushMatrix();
	modelStack.Translate(800, 0, 0);
	modelStack.Scale(300, 300, 300);
	RenderMesh(meshList[GEO_MOON], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(pelicanPos.x, pelicanPos.y, pelicanPos.z);
	modelStack.Rotate(rotation, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_PELICAN], true);
	if (rotation <= 90 && rotation >= 0)
	{
		camera.position.Set(pelicanPos.x - 150 * (1 - rotation / 90), pelicanPos.y + 80, pelicanPos.z + 150 * (rotation / 90));
	}
	if (rotation >= -90 && rotation <= 0)
	{
		camera.position.Set(pelicanPos.x + 150 * (-1 - rotation / 90), pelicanPos.y + 80, pelicanPos.z + 150 * (rotation / 90));
	}
	if (rotation > 90)
	{
		camera.position.Set(pelicanPos.x + 150 * ((rotation - 90) / 90), pelicanPos.y + 80, pelicanPos.z + 150 * (1 - (rotation - 90) / 90));
	}
	if (rotation < -90)
	{
		camera.position.Set(pelicanPos.x - 150 * ((rotation + 90) / 90), pelicanPos.y + 80, pelicanPos.z - 150 * (1 + (rotation + 90) / 90));
	}
	
	/*if (yawstore <= 90 && yawstore >= 0)
	{
		camera.position.Set(pelicanPos.x - 150 * (1 - yawstore / 90), pelicanPos.y + 80, pelicanPos.z + 150 * (yawstore / 90));
	}
	if (yawstore >= -90 && yawstore <= 0)
	{
		camera.position.Set(pelicanPos.x + 150 * (-1 - yawstore / 90), pelicanPos.y + 80, pelicanPos.z + 150 * (yawstore / 90));
	}
	if (yawstore > 90)
	{
		camera.position.Set(pelicanPos.x + 150 * ((yawstore - 90) / 90), pelicanPos.y + 80, pelicanPos.z + 150 * (1 - (yawstore - 90) / 90));
	}
	if (yawstore < -90)
	{
		camera.position.Set(pelicanPos.x - 150 * ((yawstore + 90) / 90), pelicanPos.y + 80, pelicanPos.z - 150 * (1 + (yawstore + 90) / 90));
	}*/
	camera.target.Set(pelicanPos.x, pelicanPos.y, pelicanPos.z);
	camera.up.Set(0, 1, 0);
	modelStack.PopMatrix();

	for (auto q : Object::objectMap)
	{
		for (int j = 0; j < rockpos.size(); ++j)
		{
			if ((q.first->pos.x == rockpos[j].x) &&
				(q.first->pos.z == rockpos[j].z))
			{
				modelStack.PushMatrix();
				modelStack.Translate(rockpos[j].x, rockpos[j].y, rockpos[j].z);
				modelStack.Scale(5, 5, 5);
				RenderMesh(meshList[GEO_ROCK], true);
				modelStack.PopMatrix();
			}
		}
	}

	if (!hpMid && !hpLow)
		RenderUI(meshList[GEO_HP_BAR_HIGH], 2, 10, 10, hp / 10);

	if (hpMid && !hpLow)
		RenderUI(meshList[GEO_HP_BAR_MID], 2, 10, 10, hp / 10);

	if (hpMid && hpLow)
		RenderUI(meshList[GEO_HP_BAR_LOW], 2, 10, 10, hp / 10);

	RenderUI(meshList[GEO_BORDER], 2, 10, 10, 10);
	RenderTextOnScreen(meshList[GEO_TEXT], "HP: ", Color(0, 1, 0), 2, 5, 10);

	RenderUI(meshList[GEO_SHIPDISTANCETAB], 5, 40, 60, 2);
	RenderTextOnScreen(meshList[GEO_TEXT], "DISTANCE LEFT: " + std::to_string(moonDistance - 400), Color(1, 1, 1), 1.2, 30, 57.5);

	//RenderTextOnScreen(meshList[GEO_TEXT], "Distance to reach 2nd planet : " + std::to_string(moonDistance - 400), Color(1, 0, 0), 1.5, 15, 42);

	RenderUI(meshList[GEO_CROSSHAIR], 1, 40, 30, 1);
	RenderTextOnScreen(meshList[GEO_TEXT], FPS + " FPS", Color(0, 1, 0), 1, 1, 1);	// fps
	RenderTextOnScreen(meshList[GEO_TEXT], "POSITION X: " + std::to_string(camera.position.x), Color(1, 1, 1), 1, 1, 50);
	RenderTextOnScreen(meshList[GEO_TEXT], "POSITION Z: " + std::to_string(camera.position.z), Color(1, 1, 1), 1, 1, 48);
	RenderTextOnScreen(meshList[GEO_TEXT], "Mouse X: " + std::to_string(Singleton::getInstance()->mousex), Color(1, 1, 1), 1, 1, 46);
	RenderTextOnScreen(meshList[GEO_TEXT], "Mouse Z: " + std::to_string(Singleton::getInstance()->mousey), Color(1, 1, 1), 1, 1, 44);
	RenderTextOnScreen(meshList[GEO_TEXT], "Pause Check" + std::to_string(Singleton::getInstance()->pause), Color(1, 1, 1), 1, 1, 38);
	RenderTextOnScreen(meshList[GEO_TEXT], "Mouse Speed: " + std::to_string(toupper(Singleton::getInstance()->MOUSE_SPEED)), Color(1, 1, 1), 1, 1, 28);
	if (Singleton::getInstance()->buttonText == true)
		RenderTextOnScreen(meshList[GEO_TEXT], "Button Click", Color(0, 0, 0), 1, 40, 25);

	if (Singleton::getInstance()->pause == true)
		pause();
}

void SP2Scene2::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Scale(0.8f, 0.8f, 0.8f);

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 2997.f);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(3000, 3000, 3000);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -2997.f);
	modelStack.Scale(3000, 3000, 3000);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -2997.f, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(3000, 3000, 3000);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 2997.f, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(3000, 3000, 3000);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2997.f, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(3000, 3000, 3000);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2997.f, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(3000, 3000, 3000);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();

}

void SP2Scene2::RenderText(Mesh* mesh, std::string text, Color color)
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

void SP2Scene2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void SP2Scene2::RenderUI(Mesh* mesh, float size, float x, float y, float scaleX)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 1);
	modelStack.Scale(size, size, size);
	modelStack.Scale(scaleX, 1, 1);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	mesh->Render();

	glBindTexture(GL_TEXTURE_2D, 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SP2Scene2::Reset()
{
	death = false;
	Object::objectMap.clear();
	Singleton::getInstance()->stateCheck = true;
	Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME2R;

}
void SP2Scene2::pause()
{
	RenderUI(meshList[GEO_PAUSE_BG], 5, 40, 30, 1.3);

	/////////////////////////
	//     MENU BUTTON     //
	/////////////////////////
	if ((1152 * SCREEN_WIDTH / 1920 > Singleton::getInstance()->mousex && 767 * SCREEN_WIDTH / 1920 < Singleton::getInstance()->mousex) &&
		(468 * SCREEN_HEIGHT / 1080 > Singleton::getInstance()->mousey && 398 * SCREEN_HEIGHT / 1080 <Singleton::getInstance()->mousey))
	{
		//MOUSE CLICK
		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 36, 1);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[35], Color(1, 0, 0), 1, 37, 36);
			Object::objectMap.clear();
			Singleton::getInstance()->program_state = Singleton::PROGRAM_EXIT;
		}
		//MOUSE HOVER
		else
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 36, 1);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[35], Color(1, 0, 0), 1, 37, 36);
		}
	}
	//DEFAULT
	else
	{
		RenderUI(meshList[GEO_PAUSE_BUTTONS], 1, 40, 36, 1);
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
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 30, 1);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[33], Color(1, 0, 0), 1, 38, 30);
			Application::HideCursor();
			Application::SetMousePosition(0, 0);
			Singleton::getInstance()->pause = false;
		}
		//MOUSE HOVER
		else
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 30, 1);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[33], Color(1, 0, 0), 1, 38, 30);
		}
	}
	//DEFAULT
	else
	{
		RenderUI(meshList[GEO_PAUSE_BUTTONS], 1, 40, 30, 1);
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
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 24, 1);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[34], Color(1, 0, 0), 1, 37.5, 24);
			Singleton::getInstance()->stateCheck = true;
			Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME2;
			Object::objectMap.clear();
		}
		//MOUSE HOVER
		else
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 24, 1);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[34], Color(1, 0, 0), 1, 37.5, 24);
		}
	}
	//DEFAULT
	else
	{
		RenderUI(meshList[GEO_PAUSE_BUTTONS], 1, 40, 24, 1);
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
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 18, 1);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[36], Color(1, 0, 0), 1, 38.5, 18);
			Object::objectMap.clear();
			Singleton::getInstance()->program_state = Singleton::PROGRAM_EXIT;
		}
		//MOUSE HOVER
		else
		{
			RenderUI(meshList[GEO_PAUSE_BUTTONS_HOVER], 1, 40, 18, 1);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[36], Color(1, 0, 0), 1, 38.5, 18);
		}
	}
	//DEFAULT
	else
	{
		RenderUI(meshList[GEO_PAUSE_BUTTONS], 1, 40, 18, 1);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[36], Color(1, 1, 1), 1, 38.5, 18);
	}
}
void SP2Scene2::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}