#include "Assignment2.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"

Ass2::Ass2()
{
}

Ass2::~Ass2()
{
}

void Ass2::Init()
{
	// Init VBO here
	armRotate = 60;
	inputDelay = 9.0f;
	upperJump = 6.0f;
	lowerJump = 0;
	checkArm = true;
	smile = false;
	jumping = false;
	togjump = false;

	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders("Shader//Shading.vertexshader", "Shader//LightSource.fragmentshader");
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
	glUseProgram(m_programID);

	light[0].type = Light::LIGHT_POINT;
	light[0].position.Set(0, 20, 0);
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
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	
	//ambient is the color when light is NOT shining on it
	//diffuse is the brightness of light shining on it
	//specular is the brightness of the reflected light into your camera

	//Initialize camera settings
	camera.Init(Vector3(0, 10, 20), Vector3(0, 0, 0), Vector3(0, 1, 0));

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("AXES", 1000, 1000, 1000);

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("LIGHTBALL", Color(1, 1, 1), 10, 20);

	meshList[GEO_GROUND] = MeshBuilder::GenerateCube("GROUND", Color(0.859f, 1, 1));
	meshList[GEO_GROUND]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_GROUND]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_GROUND]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_GROUND]->material.kShininess = 1.0f;

	meshList[GEO_TREE1] = MeshBuilder::GenerateCylin("TRUNK", Color(0.4f, 0.2f, 0), 18);
	meshList[GEO_TREE1]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_TREE1]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_TREE1]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_TREE1]->material.kShininess = 1.f;

	meshList[GEO_TREE2] = MeshBuilder::GenerateCone("LEAVES", Color(0.733f, 0.733f, 0.318f), 36);
	meshList[GEO_TREE2]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_TREE2]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_TREE2]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_TREE2]->material.kShininess = 1.f;

	meshList[GEO_HEAD] = MeshBuilder::GenerateSphere("HEAD", Color(0.886f, 0.729f, 0.498f), 36, 64);
	meshList[GEO_HEAD]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_HEAD]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_HEAD]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_HEAD]->material.kShininess = 1.f;

	meshList[GEO_EYE1] = MeshBuilder::GenerateSphere("EYE1", Color(1, 1, 1), 18, 36);
	meshList[GEO_EYE1]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_EYE1]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_EYE1]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_EYE1]->material.kShininess = 0.1f;

	meshList[GEO_EYE2] = MeshBuilder::GenerateSphere("EYE2", Color(0, 0, 0), 18, 36);
	meshList[GEO_EYE2]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_EYE2]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_EYE2]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_EYE2]->material.kShininess = 0.1f;

	meshList[GEO_MOUTH1] = MeshBuilder::GenerateCone("LIP", Color(0.786f, 0.629f, 0.498f), 36);
	meshList[GEO_MOUTH1]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_MOUTH1]->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_MOUTH1]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_MOUTH1]->material.kShininess = 1.f;

	meshList[GEO_MOUTH2] = MeshBuilder::GenerateSphere("MOUTH", Color(0.953f, 0.482f, 0.482f), 18, 36);
	meshList[GEO_MOUTH2]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_MOUTH2]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MOUTH2]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_MOUTH2]->material.kShininess = 0.1f;

	meshList[GEO_NOSE] = MeshBuilder::GenerateHemi("NOSE", Color(0.188f, 0.353f, 0.745f), 36, 18);
	meshList[GEO_NOSE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_NOSE]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_NOSE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_NOSE]->material.kShininess = 1.f;

	meshList[GEO_HAT1] = MeshBuilder::GenerateCylin("HAT1", Color(0.925f, 0.408f, 0.463f), 18);
	meshList[GEO_HAT1]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_HAT1]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_HAT1]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_HAT1]->material.kShininess = 1.f;

	meshList[GEO_HAT2] = MeshBuilder::GenerateTorus("HAT2", Color(0.925f, 0.408f, 0.463f), 4, 18, 0.2f, 0.5f);
	meshList[GEO_HAT2]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_HAT2]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_HAT2]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_HAT2]->material.kShininess = 1.f;

	meshList[GEO_CROSS] = MeshBuilder::GenerateCube("CROSS", Color(1, 1, 1));
	meshList[GEO_CROSS]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_CROSS]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_CROSS]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_CROSS]->material.kShininess = 1.f;

	meshList[GEO_EAR] = MeshBuilder::GenerateCone("EAR", Color(0.886f, 0.729f, 0.498f), 36);
	meshList[GEO_EAR]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_EAR]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_EAR]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_EAR]->material.kShininess = 1.f;

	meshList[GEO_HORN1] = MeshBuilder::GenerateHalfTorus("HORN1", Color(0.651f, 0.478f, 0.286f), 36, 18, 0.2f, 0.5f);
	meshList[GEO_HORN1]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_HORN1]->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_HORN1]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_HORN1]->material.kShininess = 1.f;

	meshList[GEO_HORN2] = MeshBuilder::GenerateHemi("HORN2", Color(0.651f, 0.478f, 0.286f), 36, 18);
	meshList[GEO_HORN2]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_HORN2]->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_HORN2]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_HORN2]->material.kShininess = 1.f;

	meshList[GEO_BODY] = MeshBuilder::GenerateHemi("BODY", Color(0.886f, 0.729f, 0.498f), 36, 18);
	meshList[GEO_BODY]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_BODY]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_BODY]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_BODY]->material.kShininess = 1.f;

	meshList[GEO_ARM1] = MeshBuilder::GenerateCylin("ARM", Color(0.886f, 0.729f, 0.498f), 18);
	meshList[GEO_ARM1]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_ARM1]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_ARM1]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_ARM1]->material.kShininess = 1.f;

	meshList[GEO_ARM2] = MeshBuilder::GenerateHemi("ARM PINCERS", Color(0, 0, 0), 36, 18);
	meshList[GEO_ARM2]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_ARM2]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_ARM2]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_ARM2]->material.kShininess = 0.5f;

	meshList[GEO_ARM3] = MeshBuilder::GenerateHalfHemi("LEG PINCERS", Color(0, 0, 0), 36, 18);
	meshList[GEO_ARM3]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_ARM3]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_ARM3]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_ARM3]->material.kShininess = 0.5f;

	meshList[GEO_PANT] = MeshBuilder::GenerateHemi("PANTS", Color(0.486f, 0.231f, 0.325f), 36, 18);
	meshList[GEO_PANT]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_PANT]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_PANT]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PANT]->material.kShininess = 1.f;

	meshList[GEO_CAPE1] = MeshBuilder::GenerateCone("CAPE1", Color(0.937f, 0.565f, 0.643f), 6);
	meshList[GEO_CAPE1]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_CAPE1]->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_CAPE1]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_CAPE1]->material.kShininess = 1.f;

	meshList[GEO_CAPE2] = MeshBuilder::GenerateTorus("CAPE2", Color(0.937f, 0.565f, 0.643f), 36, 18, 0.1f, 0.2f);
	meshList[GEO_CAPE2]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_CAPE2]->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_CAPE2]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_CAPE2]->material.kShininess = 1.f;

	meshList[GEO_CAPE3] = MeshBuilder::GenerateSphere("CAPE3", Color(0.937f, 0.565f, 0.643f), 18, 36);
	meshList[GEO_CAPE3]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_CAPE3]->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_CAPE3]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_CAPE3]->material.kShininess = 1.f;

	meshList[GEO_CAPE4] = MeshBuilder::GenerateCircle("CAPE4", Color(0.937f, 0.565f, 0.643f), 3);
	meshList[GEO_CAPE4]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_CAPE4]->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_CAPE4]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_CAPE4]->material.kShininess = 1.f;

	meshList[GEO_CANDY] = MeshBuilder::GenerateTorus("CANDY", Color(1, 0.729f, 0.824f), 36, 18, 0.5f, 0.4f);
	meshList[GEO_CANDY]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_CANDY]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_CANDY]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_CANDY]->material.kShininess = 1.f;

	meshList[GEO_STICK] = MeshBuilder::GenerateCone("STICK", Color(1, 1, 1), 18);
	meshList[GEO_STICK]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_STICK]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_STICK]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_STICK]->material.kShininess = 1.f;
}

static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;
static float LSPEED = 10.f;

void Ass2::Update(double dt)
{

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

	if (armRotate > 85)
	{
		checkArm = false;
	}
	else if (armRotate < 65)
	{
		checkArm = true;
	}

	if (checkArm)
	{
		armRotate += (float)(50 * dt);
	}
	else
	{
		armRotate -= (float)(50 * dt);
	}

	if (lowerJump > 7)
	{
		jumping = true;
	}
	else if (lowerJump < 0)
	{
		jumping = false;
		togjump = false;
	}

	if (Application::IsKeyPressed(VK_SPACE) && inputDelay >= 10.0f)
	{
		if (togjump == false)
		{
			togjump = true;
		}
		else
		{
			togjump = false;
		}
		inputDelay = 0;
	}
	else if (inputDelay <= 10.0f)
	{
		inputDelay += (float)(1 * dt);
	}

	if (togjump)
	{
		if (!jumping)
		{
			smile = true;
			lowerJump += (float)(15 * dt);
			upperJump += (float)(15 * dt);
		}
		if (jumping)
		{
			smile = false;
			lowerJump -= (float)(15 * dt);
			upperJump -= (float)(15 * dt);
		}
	}

	if (Application::IsKeyPressed('F') && inputDelay >= 10.0f)
	{
		if (smile == true)
		{
			smile = false;
		}
		else if (smile == false)
		{
			smile = true;
		}
		inputDelay = 0;
	}
	else if (inputDelay <= 10.0f)
	{
		inputDelay += (float)(8 * dt);
	}

	camera.Update(dt);
}

void Ass2::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);
	//load material
	glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
	glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
	glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
	glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);

	if (Application::IsKeyPressed('V'))
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
	}
	else if (Application::IsKeyPressed('B'))
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	mesh->Render();
}

void Ass2::Render()
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

	// SCALE -> ROTATE -> TRANSLATE
	// to put in code = translate -> rotate -> scale
	// push and pop scale values

	RenderMesh(meshList[GEO_AXES], false);

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -42.0f, -5);
	modelStack.Scale(120, 80, 100);
	RenderMesh(meshList[GEO_GROUND], true);
	modelStack.PopMatrix();

	for (float x = -50; x < -7; x += 13)
	{	
		for (float z = -50; z < 50; z += 13)
		{
			modelStack.PushMatrix();
			modelStack.Translate(x, 7.5f, z);
			modelStack.Scale(2, 20, 2);
			RenderMesh(meshList[GEO_TREE1], true); // trunk
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			modelStack.Translate(x, 30.0f, z);
			modelStack.Scale(5, 30, 5);
			RenderMesh(meshList[GEO_TREE2], true); // leaves
			modelStack.PopMatrix();
		}
	}
	for (float x = 11; x < 55; x += 13)
	{
		for (float z = -50; z < 50; z += 13)
		{
			modelStack.PushMatrix();
			modelStack.Translate(x, 7.5f, z);
			modelStack.Scale(2, 20, 2);
			RenderMesh(meshList[GEO_TREE1], true); // trunk
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			modelStack.Translate(x, 30.0f, z);
			modelStack.Scale(5, 30, 5);
			RenderMesh(meshList[GEO_TREE2], true); //leaves
			modelStack.PopMatrix();
		}
	}

	// --- BODY AND PANTS AND ARMS ---
	// body + pant
	modelStack.PushMatrix(); 
	modelStack.Translate(0, lowerJump, 0);
	modelStack.Scale(1.3f, 2.0f, 1.3f);
	RenderMesh(meshList[GEO_BODY], true);
	modelStack.PushMatrix();
	modelStack.Scale(1, 0.6f, 1);
	modelStack.Rotate(180, 0, 0, 1);
	RenderMesh(meshList[GEO_PANT], true);
	modelStack.PopMatrix();
	// body + pant end

	// --- CAPE ---
	modelStack.PushMatrix();
	modelStack.Scale(1.0f, 1.8f, 0.2f);
	modelStack.Translate(0, 0.3f, -5.0f);
	modelStack.Rotate(-50, 1, 0, 0);
	RenderMesh(meshList[GEO_CAPE1], true);
	modelStack.PopMatrix();
	// neck
	modelStack.PushMatrix();
	modelStack.Scale(1.6f, 0.6f, 2.3f);
	modelStack.Translate(0, 1.6f, -0.08f);
	RenderMesh(meshList[GEO_CAPE2], true);
	modelStack.PopMatrix();
	// bowtie
	modelStack.PushMatrix();
	modelStack.Scale(0.1f, 0.07f, 0.1f);
	modelStack.Translate(0, 13.0f, 5.0f);
	RenderMesh(meshList[GEO_CAPE3], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Scale(0.2f, 0.2f, 0.2f);
	modelStack.Translate(0.7f, 4.0f, 3.3f);
	modelStack.Rotate(45, 1, 0, 0);
	RenderMesh(meshList[GEO_CAPE4], true);
	modelStack.Translate(-1.4f, 0, 0);
	modelStack.Rotate(45, 0, 1, 0);
	RenderMesh(meshList[GEO_CAPE4], true);
	modelStack.PopMatrix(); 
	// ------------ CAPE END

	// --- ARMS ---
	// right
	modelStack.PushMatrix();
	modelStack.Scale(0.5f, 0.333f, 0.5f); // inverse
	modelStack.Scale(0.25f, 0.25f, 0.25f);
	modelStack.Translate(4, 9, 0);
	RenderMesh(meshList[GEO_HEAD], true); // joint
	modelStack.Rotate(-armRotate, 0, 0, 1);
	modelStack.Translate(0.0f, 4.0f, 0.0f);
	modelStack.PushMatrix();
	modelStack.Scale(0.7f, 9, 0.7f);
	RenderMesh(meshList[GEO_ARM1], true); // arm
	modelStack.PopMatrix();
	modelStack.Translate(0, 1.0f, 0);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Translate(0, -3.5f, 0);
	modelStack.PushMatrix();
	modelStack.Scale(1.7f, 1.7f, 1.7f);
	RenderMesh(meshList[GEO_BODY], true); // paw
	modelStack.PopMatrix();
	modelStack.Translate(0.0f, 0, 0);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.PushMatrix();
	modelStack.Rotate(135, 0, 0, 1);
	modelStack.Scale(1.5f, 1.5f, 1.5f);
	RenderMesh(meshList[GEO_ARM2], true); // pincers
	modelStack.Translate(0.0, 0, 0);
	modelStack.Rotate(90, 0, 0, 1);
	RenderMesh(meshList[GEO_ARM2], true); // pincers
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(1, 4, 0);
	modelStack.Rotate(-20, 0, 0, 1);
	for (int stack = 0; stack <= 3; ++stack) // how many stack of cotton candy
	{
		modelStack.Translate(0, 1, 0); // how far each stack is
		modelStack.Scale(1.7f, 1.7f, 1.7f); // how much bigger each stack will be
		RenderMesh(meshList[GEO_CANDY], true); // candy
	}

	modelStack.PushMatrix(); // stick
	modelStack.Translate(0, -1.3f, 0);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(0.3f, 1.0f, 0.3f);
	RenderMesh(meshList[GEO_STICK], true); // stick for the candy
	modelStack.PopMatrix(); // stick
	modelStack.PopMatrix(); // candy

	modelStack.PopMatrix();
	// left
	modelStack.PushMatrix();
	modelStack.Scale(0.5f, 0.333f, 0.5f); // inverse
	modelStack.Scale(0.25f, 0.25f, 0.25f);
	modelStack.Translate(-4, 9, 0);
	RenderMesh(meshList[GEO_HEAD], true); // joint
	modelStack.Rotate(armRotate, 0, 0, 1); // rotate here
	modelStack.Translate(0.0f, 4.0f, 0.0f);
	modelStack.PushMatrix();
	modelStack.Scale(0.7f, 9, 0.7f);
	RenderMesh(meshList[GEO_ARM1], true); // arm
	modelStack.PopMatrix();
	modelStack.Translate(0, 1.0f, 0);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Translate(0, -3.5f, 0);
	modelStack.PushMatrix();
	modelStack.Scale(1.7f, 1.7f, 1.7f);
	RenderMesh(meshList[GEO_BODY], true); // paw
	modelStack.PopMatrix();
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.PushMatrix();
	modelStack.Rotate(135, 0, 0, 1);
	modelStack.Scale(1.5f, 1.5f, 1.5f);
	RenderMesh(meshList[GEO_ARM2], true); // pincers
	modelStack.Rotate(90, 0, 0, 1);
	RenderMesh(meshList[GEO_ARM2], true); // pincers
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-1, 4, 0);
	modelStack.Rotate(20, 0, 0, 1);
	for (int stack = 0; stack <= 3; ++stack) // how many stack of cotton candy
	{
		modelStack.Translate(0, 1, 0); // how far each stack is
		modelStack.Scale(1.7f, 1.7f, 1.7f); // how much bigger each stack will be
		RenderMesh(meshList[GEO_CANDY], true); // candy
	}
	modelStack.PushMatrix(); // stick
	modelStack.Translate(0, -1.3f, 0);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(0.3f, 1.0f, 0.3f);
	RenderMesh(meshList[GEO_STICK], true); // stick for the candy
	modelStack.PopMatrix(); // stick
	modelStack.PopMatrix(); // candy

	modelStack.PopMatrix();
	// ------------- ARMS

	// --- LEGS ---
	modelStack.PushMatrix();
	modelStack.Scale(0.5f, 0.333f, 0.5f); // inverse
	modelStack.Scale(0.25f, 0.25f, 0.25f);
	if (togjump)
	{
		modelStack.Rotate(15, 0, 0, 1);
	}
	modelStack.Translate(3, -5.5, 0);
	RenderMesh(meshList[GEO_HEAD], true); // joint
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Translate(0.0f, 3.5f, 0.0f);
	modelStack.PushMatrix();
	modelStack.Scale(0.7f, 7, 0.7f);
	RenderMesh(meshList[GEO_ARM1], true); // leg
	modelStack.PopMatrix();
	modelStack.Translate(0, 0.5f, 0);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Translate(0, -3.5f, 0.8f);
	modelStack.PushMatrix();
	modelStack.Scale(1.7f, 1.7f, 1.7f);
	RenderMesh(meshList[GEO_BODY], true); // feet
	modelStack.PopMatrix();
	modelStack.Translate(0.0f, 0.1f, 1.7f);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.PushMatrix();
	modelStack.Rotate(135, 0, 0, 1);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1.5f, 1.5f, 1.5f);
	RenderMesh(meshList[GEO_ARM3], true); // pincers
	modelStack.Translate(0.0, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	RenderMesh(meshList[GEO_ARM3], true); // pincers
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(0.5f, 0.333f, 0.5f); // inverse
	modelStack.Scale(0.25f, 0.25f, 0.25f);
	if (togjump)
	{
		modelStack.Rotate(-15, 0, 0, 1);
	}
	modelStack.Translate(-3, -5.5, 0);
	RenderMesh(meshList[GEO_HEAD], true); // joint
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Translate(0.0f, 3.5f, 0.0f);
	modelStack.PushMatrix();
	modelStack.Scale(0.7f, 7, 0.7f);
	RenderMesh(meshList[GEO_ARM1], true); // leg
	modelStack.PopMatrix();
	modelStack.Translate(0, 0.5f, 0);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Translate(0, -3.5f, 0.8f);
	modelStack.PushMatrix();
	modelStack.Scale(1.7f, 1.7f, 1.7f);
	RenderMesh(meshList[GEO_BODY], true); // feet
	modelStack.PopMatrix();
	modelStack.Translate(0.0f, 0.1f, 1.7f);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.PushMatrix();
	modelStack.Rotate(135, 0, 0, 1);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1.5f, 1.5f, 1.5f);
	RenderMesh(meshList[GEO_ARM3], true); // pincers
	modelStack.Translate(0.0, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	RenderMesh(meshList[GEO_ARM3], true); // pincers
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	// ------------ LEGS
	modelStack.PopMatrix(); //body
	
	////////////////// --- HAT + HORN + HEAD --- ////////////////
	// hat's shelter
	modelStack.PushMatrix();
	modelStack.Translate(0, upperJump, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_HAT2], true);
	// hat's top
	modelStack.PushMatrix();
	modelStack.Scale(0.5f, 0.7f, 0.5f);
	modelStack.Translate(0, 0.7f, 0);
	RenderMesh(meshList[GEO_HAT1], true);
	modelStack.PopMatrix();
	// hat's cross
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.5f, 0.47f);
	modelStack.Rotate(45, 0, 0, 1);
	modelStack.Scale(0.1f, 0.5f, 0.1f);
	RenderMesh(meshList[GEO_CROSS], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.5f, 0.47f);
	modelStack.Rotate(-45, 0, 0, 1);
	modelStack.Scale(0.1f, 0.5f, 0.1f);
	RenderMesh(meshList[GEO_CROSS], true);
	modelStack.PopMatrix();
	// ears
	modelStack.PushMatrix();
	modelStack.Translate(0.6f, 0.3f, 0.0f);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(0.1f, 0.25f, 0.1f);
	RenderMesh(meshList[GEO_EAR], true);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Translate(0.0f, 4.8, 0.0f);
	RenderMesh(meshList[GEO_EAR], true);
	modelStack.PopMatrix();

	// ---horns---
	// left main
	modelStack.PushMatrix();
	modelStack.Translate(-0.6f, 0.75f, 0.0f);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(0.35f, 0.35f, 0.35f);
	RenderMesh(meshList[GEO_HORN1], true);
	modelStack.PopMatrix();
	// right main
	modelStack.PushMatrix();
	modelStack.Translate(0.6f, 0.75f, 0.0f);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(0.35f, 0.35f, 0.35f);
	RenderMesh(meshList[GEO_HORN1], true);
	modelStack.PopMatrix();
	// right brunch
	modelStack.PushMatrix();
	modelStack.Translate(0.85f, 1.0f, 0.0f);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(-30, 1, 0, 0);
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	RenderMesh(meshList[GEO_HORN1], true);
	modelStack.Rotate(60, 1, 0, 0);
	RenderMesh(meshList[GEO_HORN1], true);
	modelStack.PopMatrix();
	// left brunch
	modelStack.PushMatrix();
	modelStack.Translate(-0.85f, 1.0f, 0.0f);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(-30, 1, 0, 0);
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	RenderMesh(meshList[GEO_HORN1], true);
	modelStack.Rotate(60, 1, 0, 0);
	RenderMesh(meshList[GEO_HORN1], true);
	modelStack.PopMatrix();
	// cap inner brunches
	modelStack.PushMatrix();
	modelStack.Translate(-0.635f, 0.97f, 0.125f);
	modelStack.Scale(0.1f, 0.2f, 0.1f);
	RenderMesh(meshList[GEO_HORN2], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.635f, 0.97f, 0.125f);
	modelStack.Scale(0.1f, 0.2f, 0.1f);
	RenderMesh(meshList[GEO_HORN2], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.635f, 0.97f, -0.125f);
	modelStack.Scale(0.1f, 0.2f, 0.1f);
	RenderMesh(meshList[GEO_HORN2], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-0.635f, 0.97f, -0.125f);
	modelStack.Scale(0.1f, 0.2f, 0.1f);
	RenderMesh(meshList[GEO_HORN2], true);
	modelStack.PopMatrix();
	// cap outer brunches
	modelStack.PushMatrix();
	modelStack.Translate(-1.065f, 0.97f, 0.125f);
	modelStack.Scale(0.1f, 0.2f, 0.1f);
	RenderMesh(meshList[GEO_HORN2], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(1.065f, 0.97f, 0.125f);
	modelStack.Scale(0.1f, 0.2f, 0.1f);
	RenderMesh(meshList[GEO_HORN2], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(1.065f, 0.97f, -0.125f);
	modelStack.Scale(0.1f, 0.2f, 0.1f);
	RenderMesh(meshList[GEO_HORN2], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-1.065f, 0.97f, -0.125f);
	modelStack.Scale(0.1f, 0.2f, 0.1f);
	RenderMesh(meshList[GEO_HORN2], true);
	modelStack.PopMatrix();
	// ----------- HORNS

	// ---FACE---
	// head
	modelStack.PushMatrix();
	modelStack.Scale(0.5f, 0.45f, 0.45f);
	modelStack.Translate(0, -0.8f, 0);
	RenderMesh(meshList[GEO_HEAD], true);
	modelStack.PopMatrix();
	if (smile)
	{
		// smiling
		modelStack.PushMatrix();
		modelStack.Translate(0.15f, -0.35, 0.36f);
		modelStack.Rotate(-30, 1, 0, 0);
		modelStack.Rotate(20, 0, 1, 0);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Scale(0.15f, 0.04f, 0.1f);
		RenderMesh(meshList[GEO_TREE1], true);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Translate(-0.15f, -0.35, 0.36f);
		modelStack.Rotate(-30, 1, 0, 0);
		modelStack.Rotate(-20, 0, 1, 0);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Scale(0.15f, 0.04f, 0.1f);
		RenderMesh(meshList[GEO_TREE1], true);
		modelStack.PopMatrix();
	}
	else
	{
		// eye1 (white)
		modelStack.PushMatrix();
		modelStack.Scale(0.1f, 0.1f, 0.03f);
		modelStack.Translate(2.0f, -3.5f, 14.5f);
		RenderMesh(meshList[GEO_EYE1], true);
		modelStack.Translate(-4.0f, 0, 0);
		RenderMesh(meshList[GEO_EYE1], true);
		modelStack.PopMatrix();
		// eye2 (black)
		modelStack.PushMatrix();
		modelStack.Scale(0.07f, 0.07f, 0.03f);
		modelStack.Translate(2.5f, -5.0f, 15.0f);
		RenderMesh(meshList[GEO_EYE2], true);
		modelStack.Translate(-5.0f, 0, 0);
		RenderMesh(meshList[GEO_EYE2], true);
		modelStack.PopMatrix();
	}

	// nose
	modelStack.PushMatrix();
	modelStack.Scale(0.04f, 0.06f, 0.04f);
	modelStack.Translate(0, -7.0f, 11.0f);
	modelStack.Rotate(180, 0, 0, 1);
	RenderMesh(meshList[GEO_NOSE], true);
	modelStack.PopMatrix();
	// lips or moustache
	modelStack.PushMatrix();
	modelStack.Translate(-0.04, -0.47f, 0.40f);
	modelStack.Rotate(-20, 1, 0, 0);
	modelStack.Rotate(10, 0, 0, 1);
	modelStack.PushMatrix();
	modelStack.Scale(0.05f, 0.08f, 0.03f);
	RenderMesh(meshList[GEO_MOUTH1], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.04, -0.47f, 0.40f);
	modelStack.Rotate(-20, 1, 0, 0);
	modelStack.Rotate(-10, 0, 0, 1);
	modelStack.PushMatrix();
	modelStack.Scale(0.05f, 0.08f, 0.03f);
	RenderMesh(meshList[GEO_MOUTH1], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	if (smile)
	{
		// mouth
		modelStack.PushMatrix();
		modelStack.Translate(0.0f, -0.58f, 0.4f);
		modelStack.Rotate(30, 1, 0, 0);
		modelStack.Scale(0.05f, 0.1f, 0.02f);
		RenderMesh(meshList[GEO_MOUTH2], true);
		modelStack.PopMatrix();
	}
	else
	{
		// mouth
		modelStack.PushMatrix();
		modelStack.Translate(0.0f, -0.54f, 0.4f);
		modelStack.Rotate(30, 1, 0, 0);
		modelStack.Scale(0.05f, 0.06f, 0.02f);
		RenderMesh(meshList[GEO_MOUTH2], true);
		modelStack.PopMatrix();
	}
	// ---------- FACE
	modelStack.PopMatrix();

	////////////// ---------------- /////////////////// HAT AND HORN AND HEAD END
}

void Ass2::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}