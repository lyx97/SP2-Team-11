#include "Scene4.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

Scene4::Scene4()
{
}

Scene4::~Scene4()
{
}

void Scene4::Init()
{
	// Init VBO here

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

	//Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//TransformVertexShader.vertexshader", "Shader//SimpleFragmentShader.fragmentshader");
	// Use our shader
	glUseProgram(m_programID);
	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");

	//variable to rotate geometry
	rotateAngle = 0;

	//Initialize camera settings
	camera.Init(Vector3(40, 30, 30), Vector3(0, 0, 0), Vector3(0, 1, 0));

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("AXES", 1000, 1000, 1000);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("CUBE", Color(1, 1, 0));
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("SPHERE", Color(0, 1, 1), 18, 36);
	meshList[GEO_RING] = MeshBuilder::GenerateRing("RING", Color(0, 0, 1), 0.8f, 36);
	meshList[GEO_SUN] = MeshBuilder::GenerateSphere("SPHERE", Color(1, 1, 0), 18, 36);

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
}

static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;

void Scene4::Update(double dt)
{
	if (Application::IsKeyPressed('1')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	rotateAngle += (float)(10 * dt);

	camera.Update(dt);
}

void Scene4::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set view matrix using camera settings
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z, 
		camera.target.x, camera.target.y, camera.target.z, 
		camera.up.x, camera.up.y, camera.up.z);

	modelStack.LoadIdentity();

	Mtx44 MVP;

	// ---AXIS---

	modelStack.PushMatrix();
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_AXES]->Render();
	modelStack.PopMatrix();

	// ----------

	// ---SUN---
	modelStack.PushMatrix();
	modelStack.Scale(8, 8, 8);
	modelStack.Translate(0, 0, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_SUN]->Render();
	modelStack.PopMatrix();
	// ---------

	// ---PLANET AND MOON---
	modelStack.PushMatrix();
		
		modelStack.Rotate(rotateAngle, 0, 1, 0);				// rotate around origin
		modelStack.Translate(20, 0, 0);							// offset from origin

		modelStack.PushMatrix();								// moon
			modelStack.Rotate(rotateAngle, 1, 0, 0);			// revolution around the planet
			modelStack.Translate(0, 9, 0);						// offset from planet
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_SPHERE]->Render();
		modelStack.PopMatrix();

		modelStack.PushMatrix();								// planet itself
			modelStack.Rotate(10 * rotateAngle, 0, 1, 0);		// rotation around its own axis
			modelStack.PushMatrix();							// planet ring
				modelStack.Scale(6, 6, 6);						// ring size
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				meshList[GEO_RING]->Render();
			modelStack.PopMatrix();
	
		modelStack.PushMatrix();
			modelStack.Scale(4, 4, 4);
			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			meshList[GEO_SPHERE]->Render();
		modelStack.PopMatrix();

	modelStack.PopMatrix();
	// --------------------


}

void Scene4::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
