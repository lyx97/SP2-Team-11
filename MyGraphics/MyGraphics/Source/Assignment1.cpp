#include "Assignment1.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "Application.h"

Ass1::Ass1()
{
}

Ass1::~Ass1()
{
}

void Ass1::Init()
{
	// Init VBO here

	rotateAngle = 0.0f;
	translateX = 20.0f;
	translateXflag = 8.0f;
	snail = 38.0f;
	snail1 = 36.5f;
	snail2 = 33.5f;
	snail3 = 32.5f;
	trail = 1;
	cloud = 0;
	rotateAroundSun = 0;
	rotateEyes = 0;
	scaleAll = 0.0f;
	blob = true;

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// setting background color
	glClearColor(0.5294f, 0.8078f, 0.9804f, 0.0f); // RGB/A

	// generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Generate buffers
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);

	// ===SHAPES===

	// ---SQUARE---
	static const GLfloat vertex_buffer_data_SQUARE[] = {
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f
	};
	// ------------

	// ---BOAT BODY---
	static const GLfloat vertex_buffer_data_BOATBODY[] = {
		-1.5f, 1.0f, 0.0f,
		1.5f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f
	};
	// ---------------

	// ---BOAT FLAG---
	static const GLfloat vertex_buffer_data_BOATFLAG[] = {
		0.0f, 1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
	};
	// ---------------

	// ---TRIANGLE---
	static const GLfloat vertex_buffer_data_TRIANGLE[] = {
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
	};
	// --------------

	// ---CIRCLE---
	static GLfloat vertex_buffer_data_CIRCLE[360];
	for (int i = 0; i < 360; i += 3)
	{
		vertex_buffer_data_CIRCLE[i] = (float)(cos(i*(3.14f/180)));
		vertex_buffer_data_CIRCLE[i + 1] = (float)(sin(i*(3.14f/180)));
		vertex_buffer_data_CIRCLE[i + 2] = 0;
	}
	// ------------

	// ============

	// ===COLORS===
	static const GLfloat color_buffer_data_BLUE[] = {
		0.251f, 0.6431f, 0.8745f,
		0.151f, 0.5431f, 0.9745f,
		0.151f, 0.5431f, 0.9745f,
		0.251f, 0.6431f, 0.8745f,
	};
	static const GLfloat color_buffer_data_GREEN[] = {
		0.3392f, 0.7784f, 0.1667f,
		0.4392f, 0.6784f, 0.2667f,
		0.3392f, 0.7784f, 0.1667f,
		0.4392f, 0.6784f, 0.2667f,
	};
	static const GLfloat color_buffer_data_TREE[] = {
		0.5627f, 0.9196f, 0.4569f,
		0.6627f, 0.8196f, 0.5569f,
		0.6627f, 0.8196f, 0.5569f,
	};
	static const GLfloat color_buffer_data_BOATFLAG[] = {
		1.0f, 0.86f, 0.4f,
		0.9f, 0.76f, 0.5f,
		1.0f, 0.86f, 0.4f,
		0.9f, 0.76f, 0.5f,
	};
	static const GLfloat color_buffer_data_BOATBODY[] = {
		0.8294f, 0.5901f, 0.2922f,
		0.9294f, 0.4901f, 0.1922f,
		0.8294f, 0.5901f, 0.2922f,
		0.9294f, 0.4901f, 0.1922f,
	};
	static const GLfloat color_buffer_data_YELLOW1[] = {
		0.8647f, 0.8246f, 0.1784f,
		0.9647f, 0.9246f, 0.07843f,
		0.8647f, 0.8246f, 0.1784f,
		0.9647f, 0.9246f, 0.07843f,
	};
	static GLfloat color_buffer_data_YELLOW[360];
	for (int i = 0; i < 360; i += 3)
	{
		color_buffer_data_YELLOW[i] = 0.9647f;		// RED
		color_buffer_data_YELLOW[i + 1] = 0.9246f;	// GREEN
		color_buffer_data_YELLOW[i + 2] = 0.07843f;	// BLUE
	}
	static GLfloat color_buffer_data_WHITE[360];
	for (int i = 0; i < 360; i += 3)
	{
		color_buffer_data_WHITE[i] = 0.85f;			// RED
		color_buffer_data_WHITE[i + 1] = 0.85f;		// GREEN
		color_buffer_data_WHITE[i + 2] = 0.85f;		// BLUE
	}
	static GLfloat color_buffer_data_BROWN[360];
	for (int i = 0; i < 360; i += 3)
	{
		color_buffer_data_BROWN[i] = 0.498f;		// RED
		color_buffer_data_BROWN[i + 1] = 0.3765f;	// GREEN
		color_buffer_data_BROWN[i + 2] = 0.0f;		// BLUE
	}
	static GLfloat color_buffer_data_BROWN1[360];
	for (int i = 0; i < 360; i += 3)
	{
		color_buffer_data_BROWN1[i] = 0.749f;		// RED
		color_buffer_data_BROWN1[i + 1] = 0.5674f;	// GREEN
		color_buffer_data_BROWN1[i + 2] = 0.0f;		// BLUE
	}
	// ===========

	// ===LAKE===
	vertex_buffer_data_SQUARE;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_LAKE]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_SQUARE), vertex_buffer_data_SQUARE, GL_STATIC_DRAW);
	color_buffer_data_BLUE;
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_LAKE]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_BLUE), color_buffer_data_BLUE, GL_STATIC_DRAW);
	// ==========

	// ===GRASS===
	vertex_buffer_data_SQUARE;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_GRASS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_SQUARE), vertex_buffer_data_SQUARE, GL_STATIC_DRAW);
	color_buffer_data_GREEN;
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_GRASS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_GREEN), color_buffer_data_GREEN, GL_STATIC_DRAW);
	// ===========

	// ===TREE===
	vertex_buffer_data_TRIANGLE;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TREE]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_TRIANGLE), vertex_buffer_data_TRIANGLE, GL_STATIC_DRAW);
	color_buffer_data_TREE;
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TREE]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_TREE), color_buffer_data_TREE, GL_STATIC_DRAW);
	// ==========

	// ===BOAT BODY===
	vertex_buffer_data_BOATBODY;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_BOATBODY]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_BOATBODY), vertex_buffer_data_BOATBODY, GL_STATIC_DRAW);
	color_buffer_data_BOATBODY;
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_BOATBODY]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_BOATBODY), color_buffer_data_BOATBODY, GL_STATIC_DRAW);
	// ===============

	// ===BOAT POLE===
	vertex_buffer_data_SQUARE;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_BOATPOLE]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_SQUARE), vertex_buffer_data_SQUARE, GL_STATIC_DRAW);
	color_buffer_data_BOATBODY;
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_BOATPOLE]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_BOATBODY), color_buffer_data_BOATBODY, GL_STATIC_DRAW);
	// ===============

	// ===BOAT FLAG===
	vertex_buffer_data_BOATFLAG;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_BOATFLAG]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_BOATFLAG), vertex_buffer_data_BOATFLAG, GL_STATIC_DRAW);
	color_buffer_data_BOATFLAG;
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_BOATFLAG]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_BOATFLAG), color_buffer_data_BOATFLAG, GL_STATIC_DRAW);
	// ===============

	// ===SUN===
	// ---SUN ITSELF---
	vertex_buffer_data_CIRCLE;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SUN]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_CIRCLE), vertex_buffer_data_CIRCLE, GL_STATIC_DRAW);
	color_buffer_data_YELLOW;
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SUN]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_YELLOW), color_buffer_data_YELLOW, GL_STATIC_DRAW);
	// ----------------

	// ---TRIANGLE FLARES---
	vertex_buffer_data_TRIANGLE;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_FLARE]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_TRIANGLE), vertex_buffer_data_TRIANGLE, GL_STATIC_DRAW);
	color_buffer_data_YELLOW;
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_FLARE]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_YELLOW1), color_buffer_data_YELLOW1, GL_STATIC_DRAW);
	// ---------------------
	// =========

	// ===CLOUDS===
	vertex_buffer_data_CIRCLE;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_CLOUD]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_CIRCLE), vertex_buffer_data_CIRCLE, GL_STATIC_DRAW);
	color_buffer_data_WHITE;
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_CLOUD]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_WHITE), color_buffer_data_WHITE, GL_STATIC_DRAW);
	// ============

	// ===SNAIL===
	// ---SHELL---
	vertex_buffer_data_CIRCLE;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SHELL]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_CIRCLE), vertex_buffer_data_CIRCLE, GL_STATIC_DRAW);
	color_buffer_data_BROWN;
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SHELL]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_BROWN), color_buffer_data_BROWN, GL_STATIC_DRAW);
	// -----------

	// --- SHELL 2---
	vertex_buffer_data_CIRCLE;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SHELL2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_CIRCLE), vertex_buffer_data_CIRCLE, GL_STATIC_DRAW);
	color_buffer_data_BROWN1;
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SHELL2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_BROWN1), color_buffer_data_BROWN1, GL_STATIC_DRAW);
	// --- SHELL 2---

	// ---SNAIL ITSELF---
	vertex_buffer_data_SQUARE;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNAIL]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_SQUARE), vertex_buffer_data_SQUARE, GL_STATIC_DRAW);
	color_buffer_data_BROWN1;
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNAIL]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_BROWN1), color_buffer_data_BROWN1, GL_STATIC_DRAW);
	// ------------------

	//Load vertex and fragment shaders
	m_programID = LoadShaders(
		"Shader//TransformVertexShader.vertexshader", 
		"Shader//SimpleFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");

	// Use our shader
	glUseProgram(m_programID);

}

void Ass1::Update(double dt)
{

	// ---ROTATE---
	if (rotateAngle <= 160 || rotateAroundSun <= 160)
	{
		rotateAngle += (float)(7 * dt);
		rotateAroundSun += (float)(30 * dt);
	}
	else
	{
		rotateAngle = -45;
		rotateAroundSun = -45;
	}
	
	if (rotateEyes < -45)
	{
		blob = false;
	}
	else if (rotateEyes > 45)
	{
		blob = true;
	}

	if (blob)
	{
		rotateEyes -= (float)(10 * dt);
	}
	else
	{
		rotateEyes += (float)(10 * dt);
	}
	// ------------

	// --- TRANSLATE---
	if (translateX > -60 || translateXflag > -60)
	{
		translateX -= (float)(10 * dt);
		translateXflag -= (float)(10 * dt);
	}
	else
	{
		translateX = 60;
		translateXflag = 48;
	}

	if (snail < 60 || snail1 < 60 || snail2 < 60 || snail3 < 60)
	{
		snail -= (float)(1.3 * dt);
		snail1 -= (float)(1.3 * dt);
		snail2 -= (float)(1.3 * dt);
		snail3 -= (float)(1.3 * dt);
	}

	if (cloud < 60)
	{
		cloud += (float)(3 * dt);
	}
	else
	{
		cloud = -60;
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

	trail += (float)(1.3f * dt);
	// -----------

	// ---KEY PRESS---
	if (Application::IsKeyPressed(VK_SPACE))
	{
		rotateAngle = 0;
	}
	// ---------------
}

void Ass1::Render()
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

	// ===LAKE===
	scale.SetToScale(40, 9, 1);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(0, -10, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_LAKE]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_LAKE]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// ==========

	// ===GRASS===
	scale.SetToScale(40, 8, 0);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(0, -25, 1);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_GRASS]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_GRASS]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// ===========

	// ===TREE===
	for (int x = -40; x <= 40; x += 10)
	{
		for (int y = 12; y != 0; y -= 4)
		{
			scale.SetToScale(2.5f, 5, 0);
			rotate.SetToRotation(0, 0, 0, 1);
			translate.SetToTranslation(x, y, 0);
			model = translate * rotate * scale;
			MVP = projection * view * model;
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TREE]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TREE]);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}
	// =========

	// ===BOAT===
	// ---BOAT BODY---
	scale.SetToScale(11, 2.5f, 1);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(translateX, -10, 3);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_BOATBODY]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_BOATBODY]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// ---------------

	// ---BOAT POLE---
	scale.SetToScale(0.2f, 14, 1);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(translateX, 5, 3);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_BOATPOLE]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_BOATPOLE]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// ---------------

	// ---BOAT FLAG---
	scale.SetToScale(15, 12, 1);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(translateX, 7, 3);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_BOATFLAG]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_BOATFLAG]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	scale.SetToScale(15, 12, 1);
	rotate.SetToRotation(90, 0, 0, 1);
	translate.SetToTranslation(translateXflag, -5, 3);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_BOATFLAG]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_BOATFLAG]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// ---------------
	// ===========

	// ===SUN===
	// ---SUN ITSELF---
	scale.SetToScale(4, 4, 1);
	rotate.SetToRotation(rotateAngle, 0, 0, 1);
	translate.SetToTranslation(32, 23, 0);
	model = rotate * translate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SUN]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SUN]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 120);
	// ----------------

	// ---TRIANGLE FLARES---
	for (int i = 0; i <= 8; i++)
	{
		scale.SetToScale(1, 1.2f, 0);
		rotate.SetToRotation(rotateAroundSun + i * 45, 0, 0, 1);
		translate.SetToTranslation(0, 6, 0);
		model = rotate * translate * scale;
		rotate.SetToRotation(rotateAngle, 0, 0, 1);
		translate.SetToTranslation(32, 23, 0);
		model = rotate * translate * model;
		MVP = projection * view * model;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_FLARE]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_FLARE]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	// ---------------------
	// ==========

	// ===CLOUDS===
	scale.SetToScale(4, 4, 1);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(cloud, 26, 7);
	model = rotate * translate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_CLOUD]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_CLOUD]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 120);

	scale.SetToScale(4, 4, 1);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(cloud + 3, 28, 7);
	model = rotate * translate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_CLOUD]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_CLOUD]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 120);

	scale.SetToScale(4, 4, 1);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(cloud + 4, 24, 7);
	model = rotate * translate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_CLOUD]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_CLOUD]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 120);
	// ============

	// ===SNAIL===
	// ---SHELL---
	for (float size = 1.25f, zaxis = 5, yaxis = -25; size <= 3.25f; ++size, --zaxis, ++yaxis)
	{		
		scale.SetToScale(size, size, 1);
		rotate.SetToRotation(0, 0, 0, 1);
		translate.SetToTranslation(-snail, yaxis, zaxis);
		model = rotate * translate * scale;
		MVP = projection * view * model;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SHELL]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SHELL]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 120);
	}
	for (float size = 1.5f, zaxis = 4.5f, yaxis = -25; size <= 3.5f; ++size, --zaxis, ++yaxis)
	{
		scale.SetToScale(size, size, 1);
		rotate.SetToRotation(0, 0, 0, 1);
		translate.SetToTranslation(-snail, yaxis, zaxis);
		model = rotate * translate * scale;
		MVP = projection * view * model;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SHELL2]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SHELL2]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 120);
	}
	// -------------

	// ---SNAIL ITSELF---
	scale.SetToScale(5.5f, 0.7f, 1);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(-snail1, -27, 3.5f);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNAIL]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNAIL]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	scale.SetToScale(0.3f, 1.5f, 1);
	rotate.SetToRotation(rotateEyes, 0, 0, 1);
	translate.SetToTranslation(-snail2, -25.7f, 3.5f);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNAIL]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNAIL]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	scale.SetToScale(0.3f, 1.5f, 1);
	rotate.SetToRotation(rotateEyes, 0, 0, 1);
	translate.SetToTranslation(-snail3, -25.7f, 3.5f);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNAIL]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNAIL]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// ------------------

	// ---TRAIL---
	scale.SetToScale(trail, 0.3f, 1);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(-37.5, -27, 3.5f);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNAIL]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNAIL]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// -----------
	// ===========

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Ass1::Exit()
{
	// Cleanup VBO here
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}