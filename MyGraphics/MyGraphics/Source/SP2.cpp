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

SP2::SP2()
{
}

SP2::~SP2()
{
}

void SP2::Init()
{
	// Init VBO here
	Singleton::getInstance()->pause = false;
	Singleton::getInstance()->buttonText = false;
	inputDelay = 9.0f;
	board = false;
	PlanePos.Set(10, 0, 50);
	startingPlane.planePos = Vector3(0, 0, 0);
	startingPlane.planeMin = Vector3(0, 0, 0);
	startingPlane.planeMax = Vector3(300, 0, 300);
    planeInit();
	for (int loop = 0; loop < oreFrequency; loop++)
	{
		srand(rand() % 100 - 1);
		orePos.push_back(Vector3(rand() % 800 - 1, 0, rand() % 800 - 1));
	}

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
	camera.Init(Vector3(150, 7, 150), Vector3(89, 0, 0), Vector3(0, 1, 0));

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("AXES", 500, 500, 500);

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("LIGHTBALL", Color(1, 1, 1), 10, 20);

	meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("GROUND", Color(0.3f, 0.3f, 0.3f), TexCoord(10, 10), 1, 1);
	meshList[GEO_GROUND]->textureID = LoadTGA("Image//planet1_land.tga");

	meshList[GEO_HOUSE2] = MeshBuilder::GenerateCube("WALLS", Color(0.3f, 0.3f, 0.3f));

	meshList[GEO_ORE] = MeshBuilder::GenerateOBJ("ORE", "OBJ//Ore.obj");
	meshList[GEO_ORE]->textureID = LoadTGA("Image//TinOre.tga");

	meshList[GEO_GUN] = MeshBuilder::GenerateOBJ("ATATWALKER", "OBJ//gun3.obj");
	meshList[GEO_GUN]->textureID = LoadTGA("Image//gun3.tga");

	meshList[GEO_SWORD] = MeshBuilder::GenerateOBJ("ATATWALKER", "OBJ//sword.obj");
	meshList[GEO_SWORD]->textureID = LoadTGA("Image//sword.tga");

	meshList[GEO_IMAGES] = MeshBuilder::GenerateQuad("images", Color(1, 1, 1), TexCoord(1, 1), 1, 1);
	meshList[GEO_IMAGES]->textureID = LoadTGA("Image//images.tga");

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
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//heh.tga");

	meshList[GEO_HITBOX] = MeshBuilder::GenerateCube("HITBOX", Color(1, 0, 0));

	for (auto q : orePos)
	{
		//cout << q.x << " " << q.y << " " << q.z << endl;
		ore = new Object(Vector3(q.x, 7, q.z), Vector3(10, 15, 10));
	}
 	//cout << ore->hitbox.minPt << " " << ore->hitbox.maxPt << endl;
}

static float LSPEED = 10.f;
static float HOVER_SPEED = 10.f;
std::string FPS;

void SP2::Update(double dt)
{
    if (Singleton::getInstance()->pause == true)
    {
        if (Application::IsKeyPressed('O'))
        {
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

		for (auto q : Object::objectVec)
		{
			if (Application::IsKeyPressed('E') && q->hitbox.isTouching(camera.target))
			{
				Inventory::addObject(ore);
				delete q;
			}
		}

	}
    if (inputDelay <= 10.0f)
    {
        inputDelay += (float)(1 * dt);
    }

    else
    {
        camera.Update(dt);
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
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	for (int i = 0; i < Object::objectVec.size(); i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(Object::objectVec[i]->pos.x, Object::objectVec[i]->pos.y, Object::objectVec[i]->pos.z);
		modelStack.Scale(Object::objectVec[i]->size.x, Object::objectVec[i]->size.y, Object::objectVec[i]->size.z);
		//RenderMesh(meshList[GEO_HITBOX], false);
		modelStack.PopMatrix();
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

	for (auto pos : orePos)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0 + pos.x, 0, 0 + pos.z);
		modelStack.Scale(4, 4, 4);
		RenderMesh(meshList[GEO_ORE], true);
		modelStack.PopMatrix();
	}

	RenderUI(meshList[GEO_IMAGES], 10, 7, 3);
	RenderTextOnScreen(meshList[GEO_TEXT], FPS + " FPS", Color(0, 1, 0), 1, 1, 1);	// fps
	RenderTextOnScreen(meshList[GEO_TEXT], "POSITION X: " + std::to_string(camera.position.x), Color(0, 0, 0), 1, 1, 50);
	RenderTextOnScreen(meshList[GEO_TEXT], "POSITION Z: " + std::to_string(camera.position.z), Color(0, 0, 0), 1, 1, 48);
	RenderTextOnScreen(meshList[GEO_TEXT], "Mouse X: " + std::to_string(camera.mousex), Color(0, 0, 0), 1, 1, 46);
	RenderTextOnScreen(meshList[GEO_TEXT], "Mouse Z: " + std::to_string(camera.mousey), Color(0, 0, 0), 1, 1, 44);
	RenderTextOnScreen(meshList[GEO_TEXT], "distance sword " + std::to_string(distanceSword), Color(0, 0, 0), 1, 1, 42);
	RenderTextOnScreen(meshList[GEO_TEXT], "distance gun " + std::to_string(distanceGun), Color(0, 0, 0), 1, 1, 40);
	RenderTextOnScreen(meshList[GEO_TEXT], "Pause Check" + std::to_string(Singleton::getInstance()->pause), Color(0, 0, 0), 1, 1, 38);
	for (auto q : Singleton::getInstance()->objectCount)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Ores: " + std::to_string(q.second), Color(0, 0, 0), 1, 1, 36);
	}
	RenderTextOnScreen(meshList[GEO_TEXT], ">0<", Color(1.0f, 0, 0), 1, 40, 30);	// crosshair

	if (Singleton::getInstance()->buttonText == true)
		RenderTextOnScreen(meshList[GEO_TEXT], "Button Click", Color(0, 0, 0), 1, 40, 25);
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
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);
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
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
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

void SP2::RenderUI(Mesh* mesh, float size, float x, float y)
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
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 1);
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

void SP2::planeInit(){

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

void SP2::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}