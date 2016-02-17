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

	inputDelay = 9.0f;
	board = false;
	PlanePos.Set(10, 0, 50);
	startingPlane.planePos = Vector3(0, 0, 0);
	startingPlane.planeMin = Vector3(0, 0, 0);
	startingPlane.planeMax = Vector3(300, 0, 300);
	planesList.push_back(startingPlane);
	for (int loop = 0; loop < oreFrequency; loop++)
	{
		srand(rand() % 100 - 1);
		orePos.push_back(Vector3(rand() % 800 - 1, rand() % 800 - 1, rand() % 800 - 1));
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
	light[0].power = 1;
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
	camera.Init(Vector3(90, 0, 0), Vector3(89, 0, 0), Vector3(0, 1, 0));

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("AXES", 500, 500, 500);

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("LIGHTBALL", Color(1, 1, 1), 10, 20);

	meshList[GEO_GLASS] = MeshBuilder::GenerateQuad("GLASS", Color(0.3f, 0.3f, 0.3f), TexCoord(1,1));
    meshList[GEO_GLASS]->textureID = LoadTGA("Image//planet1_land.tga");


	meshList[GEO_HOUSE1] = MeshBuilder::GenerateQuad("FLOOR", Color(0.3f, 0.3f, 0.3f), TexCoord(1, 1));
	meshList[GEO_HOUSE1]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_HOUSE1]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_HOUSE1]->material.kSpecular.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_HOUSE1]->material.kShininess = 0.5f;
	meshList[GEO_HOUSE1]->textureID = LoadTGA("Image//floor.tga");

	meshList[GEO_HOUSE2] = MeshBuilder::GenerateCube("WALLS", Color(0.3f, 0.3f, 0.3f));

	meshList[GEO_ATAT] = MeshBuilder::GenerateOBJ("ATATWALKER", "OBJ//ground_UV.obj");
	meshList[GEO_ATAT]->textureID = LoadTGA("Image//ground_UV.tga");

	meshList[GEO_PELICAN] = MeshBuilder::GenerateOBJ("PELICAN", "OBJ//air.obj");
	meshList[GEO_PELICAN]->textureID = LoadTGA("Image//air_UV.tga");

	meshList[GEO_ORE] = MeshBuilder::GenerateOBJ("ORE", "OBJ//Ore.obj");
	meshList[GEO_ORE]->textureID = LoadTGA("Image//TinOre.tga");

	meshList[GEO_GUN] = MeshBuilder::GenerateOBJ("ATATWALKER", "OBJ//gun3.obj");
	meshList[GEO_GUN]->textureID = LoadTGA("Image//gun3.tga");

	meshList[GEO_SWORD] = MeshBuilder::GenerateOBJ("ATATWALKER", "OBJ//sword.obj");
	meshList[GEO_SWORD]->textureID = LoadTGA("Image//sword.tga");

	meshList[GEO_IMAGES] = MeshBuilder::GenerateQuad("images", Color(1, 1, 1), TexCoord(1, 1));
	meshList[GEO_IMAGES]->textureID = LoadTGA("Image//images.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("FRONT", Color(0, 0, 0), TexCoord(1, 1));
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//planet1_ft.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("BACK", Color(0, 0, 0), TexCoord(1, 1));
	meshList[GEO_BACK]->textureID = LoadTGA("Image//planet1_bk.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("TOP", Color(0, 0, 0), TexCoord(1, 1));
	meshList[GEO_TOP]->textureID = LoadTGA("Image//planet1_up.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("BOTTOM", Color(0, 0, 0), TexCoord(1, 1));
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//planet1_dn.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("RIGHT", Color(0, 0, 0), TexCoord(1, 1));
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//planet1_rt.tga");
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("LEFT", Color(0, 0, 0), TexCoord(1, 1));
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//planet1_lf.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//heh.tga");

	meshList[GEO_HITBOX] = MeshBuilder::GenerateCube("HITBOX", Color(1, 1, 1));
	test->setPos(0, 0, 0);
	test->setSize(20, 20, 20);
}

static float LSPEED = 10.f;
static float HOVER_SPEED = 10.f;
std::string FPS;

void SP2::Update(double dt)
{
	//std::cout << camera.position << std::endl;
	//cout << test->hitbox.minPt << " : " << test->hitbox.maxPt << endl;
	if (Application::IsKeyPressed('1')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	if (Application::IsKeyPressed('I'))
		light[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[0].position.y += (float)(LSPEED * dt);

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
	
	distanceSword = sqrtf((0.f - camera.position.x) * (0.f - camera.position.x) + (0.f - camera.position.z) * (0.f - camera.position.z));
	distanceGun = sqrtf((-50.f - camera.position.x) * (-50.f - camera.position.x) + (0.f - camera.position.z) * (0.f - camera.position.z));

	if (distanceSword < 10) collideText = true;
	else if (distanceGun < 10) collideText = true;
	else collideText = false;


	if (inputDelay <= 10.0f)
	{
		inputDelay += (float)(1 * dt);
	}

	if (Application::IsKeyPressed('E') && planeHitbox(camera.position) == true && inputDelay >= 10.f)
	{
		camera.target = PlanePos;
		camera.view = Vector3(1, 0, 0);
		camera.position = camera.target - camera.view * 100;
		camera.up = Vector3(0, 1, 0);
		camera.right = camera.view.Cross(camera.up);
		camera.right.Normalized();

		inputDelay = 0;
		board = true;
	}

    //Check which plane player is standing on
	for (auto planeIt : planesList){
		if ((camera.position.x <= planeIt.planeMax.x && camera.position.z <= planeIt.planeMax.z) && (camera.position.x >= planeIt.planeMin.x && camera.position.z >= planeIt.planeMin.z)){
			currPlane = planeIt;
		}
	}



    if (planesList.size() > 4){
        planesList.pop_front();

    }


    //Generate plane to the  of the current plane
		if (camera.position.z > currPlane.planeMax.z - 40){
			plane newPlane;
            //int offset = 0;
            //if (planesList.size() == 1){
            //    offset = 300;
            //}
            //else{
            //    offset = 0;
            //}
			newPlane.planeMin = currPlane.planeMax - Vector3(300,0,0);
			newPlane.planeMax = Vector3(newPlane.planeMin.x + 300, 0, newPlane.planeMin.z + 300);
            newPlane.planePos = newPlane.planeMin;
			planesList.push_back(newPlane);
           
		}


        if (camera.position.z < currPlane.planeMin.z + 40){
            plane newPlane;
            //int offset = 0;
            //if (planesList.size() == 1){
            //    offset = 300;
            //}
            //else{
            //    offset = 0;
            //}
            newPlane.planeMax = currPlane.planeMin + Vector3(300, 0, 0);
            newPlane.planeMin = Vector3(newPlane.planeMax.x - 300, 0, newPlane.planeMax.z - 300);
            newPlane.planePos = newPlane.planeMin;

            planesList.push_back(newPlane);
        }

        if (camera.position.x > currPlane.planeMax.x - 40){
            plane newPlane;
            //int offset = 0;
            //if (planesList.size() == 1){
            //    offset = 300;
            //}
            //else{
            //    offset = 0;
            //}
            newPlane.planeMin = currPlane.planeMin + Vector3(300,0,0);
            newPlane.planeMax = Vector3(newPlane.planeMin.x + 300, 0, newPlane.planeMin.z + 300);
            newPlane.planePos = newPlane.planeMin;

            planesList.push_back(newPlane);

        }

        if (camera.position.x < currPlane.planeMin.x + 40){
            plane newPlane;
            //int offset = 0;
            //if (planesList.size() == 1){
            //    offset = 300;
            //}
            //else{
            //    offset = 0;
            //}
            newPlane.planeMin = currPlane.planeMin - Vector3(300, 0, 0);
            newPlane.planeMax = Vector3(newPlane.planeMin.x + 300, 0, newPlane.planeMin.z + 300);
            newPlane.planePos = newPlane.planeMin;

            planesList.push_back(newPlane);

        }



        std::cout << "plane pos" << startingPlane.planePos << std::endl;
        std::cout << planesList.size() << std::endl;

	if (Application::IsKeyPressed('E') && board == true && inputDelay >= 10.f)
	{
		camera.position = PlanePos;
		camera.view = Vector3(1, 0, 0);
		camera.target = camera.position + camera.view;
		camera.up = Vector3(0, 1, 0);
		camera.right = camera.view.Cross(camera.up);
		camera.right.Normalized();

		inputDelay = 0;
		board = false;
	}

	if (board == true)
	{
		camera.EnterShip(PlanePos, dt);
	}
	else
	{
		camera.Update(dt, Object::objectVec);
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
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		modelStack.Translate(Object::objectVec[i]->pos.x, Object::objectVec[i]->pos.y, Object::objectVec[i]->pos.z);
		modelStack.Scale(Object::objectVec[i]->size.x, Object::objectVec[i]->size.y, Object::objectVec[i]->size.z);
		RenderMesh(meshList[GEO_HITBOX], false);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		modelStack.PopMatrix();
	}

	for (auto planeIt : planesList)
	{
		modelStack.PushMatrix();
		modelStack.Translate(planeIt.planePos.x, -7, planeIt.planePos.z);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(300, 300, 1);
		RenderMesh(meshList[GEO_GLASS], true);
		modelStack.PopMatrix();
	}
	for (auto pos : orePos){
		modelStack.PushMatrix();
		modelStack.Translate(0 + pos.x, -7, 0 + pos.z);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(4, 4, 4);
		//RenderMesh(meshList[GEO_ORE], true);
		modelStack.PopMatrix();
	}
  
	modelStack.PushMatrix();
	modelStack.Translate(90, -6.8, 70);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(30, 30, 1);
	RenderMesh(meshList[GEO_HOUSE1], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -7.5, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_ATAT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(PlanePos.x, PlanePos.y, PlanePos.z);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_PELICAN], false);
	modelStack.PopMatrix();

	RenderTextOnScreen(meshList[GEO_TEXT], FPS + " FPS", Color(0, 1, 0), 1, 1, 1);	// fps
	RenderTextOnScreen(meshList[GEO_TEXT], "POSITION X: " + std::to_string(camera.position.x), Color(0, 0, 0), 1, 1, 50);
	RenderTextOnScreen(meshList[GEO_TEXT], "POSITION Z: " + std::to_string(camera.position.z), Color(0, 0, 0), 1, 1, 48);
	RenderTextOnScreen(meshList[GEO_TEXT], "Mouse X: " + std::to_string(camera.up.x), Color(0, 0, 0), 1, 1, 46);
	RenderTextOnScreen(meshList[GEO_TEXT], "Mouse Z: " + std::to_string(camera.up.y), Color(0, 0, 0), 1, 1, 44);
	RenderTextOnScreen(meshList[GEO_TEXT], "distance sword " + std::to_string(distanceSword), Color(0, 0, 0), 1, 1, 42);
	RenderTextOnScreen(meshList[GEO_TEXT], "distance gun " + std::to_string(distanceGun), Color(0, 0, 0), 1, 1, 40);
	RenderTextOnScreen(meshList[GEO_TEXT], ">0<", Color(1.0f, 0, 0), 1, 40, 30);	// crosshair

	if (collideText)
		RenderTextOnScreen(meshList[GEO_TEXT], "Collide!!!", Color(0, 0, 0), 1, 40, 25);
}

void SP2::RenderSkybox()
{
    modelStack.PushMatrix();
    modelStack.Scale(0.20f, 0.20f, 0.20f);

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
void SP2::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}