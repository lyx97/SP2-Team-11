#include "Scene2.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "Application.h"

Scene2::Scene2()
{
}

Scene2::~Scene2()
{
}

void Scene2::Init()
{
	// Init VBO here

	rotateAngle = 0.0f;
	translateX = 0.0f;
	scaleAll = 0.0f;


	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// setting background color
	glClearColor(0.0f, 0.5f, 0.0f, 0.0f); // RGB/A

	// generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Generate buffers
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);

	// ---TRIANGLE 1---

	// An array of 3 vectors which represents 3 vertices
	static const GLfloat vertex_buffer_data1[] = {
		-1.0f, 1.0f, 0.0f, //vertex 0 of triangle
		1.0f, 1.0f, 0.0f, //vertex 1 of triangle
		0.0f, -1.0f, 0.0f, //vertex 2 of triangle
	};

	// An array of 3 vectors which represents the colors of the 3 vertices
	static const GLfloat color_buffer_data1[] = {
		0.5f, 0.5f, 0.5f, //vertex 0 of triangle
		0.5f, 0.5f, 0.5f, //vertex 1 of triangle
		0.5f, 0.5f, 0.5f, //vertex 2 of triangle
	};

	// Set the current active buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);

	// Transfer vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data1), vertex_buffer_data1, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data1), color_buffer_data1, GL_STATIC_DRAW);

	// -----------------

	//  ---TRIANGLE 2---

	//static const GLfloat vertex_buffer_data2[] = {
	//	0.0f, 1.0f, 0.0f,
	//	-1.0f, -1.0f, 0.0f,
	//	1.0f, -1.0f, 0.0f,
	//};

	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_2]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data2), vertex_buffer_data2, GL_STATIC_DRAW);

	//static const GLfloat color_buffer_data2[] = {
	//	1.0f, 1.0f, 0.0f,
	//	1.0f, 1.0f, 0.0f,
	//	1.0f, 1.0f, 0.0f,
	//};

	//glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_2]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data2), color_buffer_data2, GL_STATIC_DRAW);

	// -----------------

	//Load vertex and fragment shaders
	m_programID = LoadShaders(
		"Shader//TransformVertexShader.vertexshader", 
		"Shader//SimpleFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");

	// Use our shader
	glUseProgram(m_programID);

}

void Scene2::Update(double dt)
{

	// ---ROTATE---
	if (rotateAngle < 45)
	{
		rotateAngle += (float)(100 * dt);
	}
	else if (rotateAngle > 0)
	{
		rotateAngle -= (float)(100 * dt);
	}

	// ------------

	// --- TRANSLATE---
	if (translateX > -30)
	{
		translateX -= (float)(80 * dt);
	}
	else
	{
		translateX = 30;
	}

	// ----------------

	// ---SCALE---

	if (scaleAll < 20)
	{
		scaleAll += (float)(20 * dt);
	}
	else
	{
		scaleAll = 0;
	}

	// -----------

	if (Application::IsKeyPressed(VK_SPACE))
	{
		rotateAngle = 0;
	}

}

void Scene2::Render()
{
	// Render VBO here

	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mtx44 translate, rotate, scale;
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;
	Mtx44 MVP;
	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();
	model.SetToIdentity();
	view.SetToIdentity();
	projection.SetToOrtho(-40, 40, -30, 30, -10, 10);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// ---TRIANGLE 1---

	scale.SetToScale(5, 5, 5);
	rotate.SetToRotation(rotateAngle, 0, 0, 1);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	rotate.SetToRotation(90, 0, 0, 1);
	translate.SetToTranslation(translateX, 20, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	scale.SetToScale(5, scaleAll, 5);
	translate.SetToTranslation(5, 5, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// -----------------

	//  ---TRIANGLE 2---

	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_2]);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_2]);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); // yay i can comment

	//glDrawArrays(GL_TRIANGLES, 0, 3);

	// -----------------

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Scene2::Exit()
{
	// Cleanup VBO here
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}