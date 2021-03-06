/****************************************************************************/
/*!
\file MainMenu.cpp
\author Teo Jia Hao
\par email: kelvinteojiahao\@hotmail.com
\brief
Class to define an MainMenu
*/
/****************************************************************************/
#include "MainMenu.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "MeshBuilder.h"

#include "Utility.h"
#include "LoadTGA.h"

#include "Application.h"
#include "Singleton.h"
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
MainMenu::MainMenu()
{
	state = MENU_MAIN;
}
/******************************************************************************/
/*!
\brief
Default destructor

\exception None
\return None
*/
/******************************************************************************/
MainMenu::~MainMenu()
{

}
/******************************************************************************/
/*!
\brief
Initializes for MainMenu, any booleans/strings/floats and meshes

\exception None
\return None
*/
/******************************************************************************/
void MainMenu::Init()
{
	sound.playMusic("Music//Music.mp3");
	glClearColor(1.0f, 0.86f, 0.79f, 0.0f);

	textFile("Text//Menu.txt");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f);
	projectionStack.LoadMatrix(projection);

	//Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

	//Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	//Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	// Use our shader
	glUseProgram(m_programID);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//font.tga");

	meshList[GEO_BACKGROUND] = MeshBuilder::GenerateQuad("images", Color(1, 1, 1), TexCoord(1, 1), 5, 4);
	meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//background.tga");

	meshList[GEO_BACKGROUND2] = MeshBuilder::GenerateQuad("images", Color(0, 0, 0), TexCoord(1, 1), 3.5, 2.5);
	meshList[GEO_BACKGROUND2]->textureID = LoadTGA("Image//background2.tga");

	meshList[GEO_BUTTON] = MeshBuilder::GenerateQuad("images", Color(1, 1, 1), TexCoord(1, 1), 8, 2);
	meshList[GEO_BUTTON]->textureID = LoadTGA("Image//buttonDefault.tga");

	meshList[GEO_BUTTON_HOVER] = MeshBuilder::GenerateQuad("images", Color(1, 1, 1), TexCoord(1, 1), 8, 2);
	meshList[GEO_BUTTON_HOVER]->textureID = LoadTGA("Image//buttonHover.tga");
}
/******************************************************************************/
/*!
\brief
Handles all switch cases for MainMenu and animation

\exception None
\return None
*/
/******************************************************************************/
void MainMenu::Update(double dt)
{
	switch (state)
	{
	case MainMenu::MENU_MAIN:mainMenu();
		break;
	case MainMenu::MENU_INSTRUCTIONS:instruction();
		break;
	case MainMenu::MENU_CREDITS:credits();
		break;
	case MainMenu::MENU_EXIT:
		break;
	}
	if (!animation)
	{
		animationMove += (float)(200 * dt);
		if (animationMove >= 115)
			animationMove = 115;
	}
	else
	{
		animationMove -= (float)(200 * dt);
		if (animationMove <= 40)
			animationMove = 40;
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
void MainMenu::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mtx44 MVP;

	modelStack.LoadIdentity();
	RenderUI(meshList[GEO_BACKGROUND], 10, 40, 30);

	switch (state)
	{
	case MainMenu::MENU_MAIN:mainMenu();
		break;
	case MainMenu::MENU_START:
		break;
	case MainMenu::MENU_INSTRUCTIONS:instruction();
		break;
	case MainMenu::MENU_CREDITS:credits();
		break;
	case MainMenu::MENU_EXIT:
		break;
	}
	RenderTextOnScreen(meshList[GEO_TEXT], "Mouse X: " + std::to_string(Singleton::getInstance()->mousex), Color(1, 1, 1), 1, 1, 4);
	RenderTextOnScreen(meshList[GEO_TEXT], "Mouse Y: " + std::to_string(Singleton::getInstance()->mousey), Color(1, 1, 1), 1, 1, 2);
}
/******************************************************************************/
/*!
\brief
Pushes in the textfile into array

\exception None
\return None
*/
/******************************************************************************/
void MainMenu::textFile(string filename)
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
void MainMenu::RenderMesh(Mesh *mesh, bool enableLight)
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
Renders obj and targa on screen

\param mesh
Takes in which mesh to render out
\param size
Scale of the mesh to render out
\param x and y
The position of the mesh

\exception None
\return None
*/
/******************************************************************************/
void MainMenu::RenderUI(Mesh* mesh, float size, float x, float y)
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

\exception None
\return None
*/
/******************************************************************************/
void MainMenu::RenderUIwithTranparent(Mesh* mesh, float size, float x, float y)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode

	glBlendFunc(1, 1.5f);
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 1);
	modelStack.Scale(size, size, size);
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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
}
/******************************************************************************/
/*!
\brief
Renders text

\exception None
\return None
*/
/******************************************************************************/
void MainMenu::RenderText(Mesh* mesh, std::string text, Color color)
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
void MainMenu::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
Render mainmenu

\exception None
\return None
*/
/******************************************************************************/
void MainMenu::mainMenu()
{
	//////////////////////////
	//     START BUTTON     //
	/////////////////////////
	RenderTextOnScreen(meshList[GEO_TEXT], my_arr[0], Color(1, 1, 1), 5, 15, 45);

	if ((1152 * SCREEN_WIDTH / 1920 > Singleton::getInstance()->mousex && 767 * SCREEN_WIDTH / 1920 < Singleton::getInstance()->mousex) &&
		(575 * SCREEN_HEIGHT / 1080 > Singleton::getInstance()->mousey && 505 * SCREEN_HEIGHT / 1080 <Singleton::getInstance()->mousey))
	{
		//MOUSE CLICK
		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			sound.stopMusic("Music//Music.mp3");
			RenderUI(meshList[GEO_BUTTON_HOVER], 1, 40, 30);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[2], Color(1, 0, 0), 1, 37.5, 30);
			Singleton::getInstance()->stateCheck = true;
			Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME;
		}
		//MOUSE HOVER
		else
		{
			RenderUI(meshList[GEO_BUTTON_HOVER], 1, 40, 30);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[1], Color(1, 0, 0), 1, 38.5, 30);
		}
	}
	//DEFAULT
	else
	{
		RenderUI(meshList[GEO_BUTTON], 1, 40, 30);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[1], Color(1, 1, 1), 1, 38.5, 30);
	}

	////////////////////////////////
	//     INSTRUCTION BUTTON     //
	////////////////////////////////
	if ((1152 * SCREEN_WIDTH / 1920 > Singleton::getInstance()->mousex && 767 * SCREEN_WIDTH / 1920 < Singleton::getInstance()->mousex) &&
		(665 * SCREEN_HEIGHT / 1080 > Singleton::getInstance()->mousey && 595 * SCREEN_HEIGHT / 1080 <Singleton::getInstance()->mousey))
	{
		//MOUSE CLICK
		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			RenderUI(meshList[GEO_BUTTON_HOVER], 1, 40, 25);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[3], Color(1, 0, 0), 1, 35.5, 25);
			state = MENU_INSTRUCTIONS;
			animation = true;
		}
		//MOUSE HOVER
		else
		{
			RenderUI(meshList[GEO_BUTTON_HOVER], 1, 40, 25);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[3], Color(1, 0, 0), 1, 35.5, 25);
		}
	}
	//DEFAULT
	else
	{
		RenderUI(meshList[GEO_BUTTON], 1, 40, 25);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[3], Color(1, 1, 1), 1, 35.5, 25);
	}

	////////////////////////////
	//     CREDITS BUTTON     //
	////////////////////////////
	if ((1152 * SCREEN_WIDTH / 1920 > Singleton::getInstance()->mousex && 767 * SCREEN_WIDTH / 1920 < Singleton::getInstance()->mousex) &&
		(755 * SCREEN_HEIGHT / 1080 > Singleton::getInstance()->mousey && 685 * SCREEN_HEIGHT / 1080 <Singleton::getInstance()->mousey))
	{
		//MOUSE CLICK
		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			RenderUI(meshList[GEO_BUTTON_HOVER], 1, 40, 20);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[5], Color(1, 0, 0), 1, 37.5, 20);
			state = MENU_CREDITS;
			animation = true;
		}
		//MOUSE HOVER
		else
		{
			RenderUI(meshList[GEO_BUTTON_HOVER], 1, 40, 20);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[5], Color(1, 0, 0), 1, 37.5, 20);
		}
	}
	//DEFAULT
	else
	{
		RenderUI(meshList[GEO_BUTTON], 1, 40, 20);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[5], Color(1, 1, 1), 1, 37.5, 20);
	}

	/////////////////////////
	//     EXIT BUTTON     //
	/////////////////////////
	if ((1152 * SCREEN_WIDTH / 1920 > Singleton::getInstance()->mousex && 767 * SCREEN_WIDTH / 1920 < Singleton::getInstance()->mousex) &&
		(845 * SCREEN_HEIGHT / 1080 > Singleton::getInstance()->mousey && 775 * SCREEN_HEIGHT / 1080 <Singleton::getInstance()->mousey))
	{
		//MOUSE CLICK
		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			RenderUI(meshList[GEO_BUTTON_HOVER], 1, 40, 15);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[6], Color(1, 0, 0), 1, 37.5, 15);
			Singleton::getInstance()->program_state = Singleton::PROGRAM_EXIT;
		}
		//MOUSE HOVER
		else
		{
			RenderUI(meshList[GEO_BUTTON_HOVER], 1, 40, 15);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[6], Color(1, 0, 0), 1, 37.5, 15);
		}
	}
	//DEFAULT
	else
	{
		RenderUI(meshList[GEO_BUTTON], 1, 40, 15);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[6], Color(1, 1, 1), 1, 37.5, 15);
	}
}
/******************************************************************************/
/*!
\brief
Render instruction

\exception None
\return None
*/
/******************************************************************************/
void MainMenu::instruction()
{
	RenderUIwithTranparent(meshList[GEO_BACKGROUND2], 10, animationMove, 30);
	RenderTextOnScreen(meshList[GEO_TEXT], my_arr[3], Color(0, 0, 0), 3, animationMove - 15, 45);

	int j = 40;
	for (int i = 8; i <= 14; ++i)
	{
		j -= 4;
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[i], Color(0, 0, 0), 1.5, animationMove -17, j);
	}

	//////////////////////////
	//     BACK BUTTON     //
	/////////////////////////

	if ((1632 * SCREEN_WIDTH / 1920 > Singleton::getInstance()->mousex && 1248 * SCREEN_WIDTH / 1920 < Singleton::getInstance()->mousex) &&
		(935 * SCREEN_HEIGHT / 1080 > Singleton::getInstance()->mousey && 865 * SCREEN_HEIGHT / 1080 <Singleton::getInstance()->mousey))
	{
		//MOUSE CLICK
		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			animation = false;
			RenderUI(meshList[GEO_BUTTON_HOVER], 1, animationMove + 20, 10);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[7], Color(1, 0, 0), 1, animationMove + 19, 10);
			state = MENU_MAIN;

		}
		//MOUSE HOVER
		else
		{
			RenderUI(meshList[GEO_BUTTON_HOVER], 1, animationMove + 20, 10);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[7], Color(1, 0, 0), 1, animationMove + 19, 10);
		}
	}
	//DEFAULT
	else
	{
		RenderUI(meshList[GEO_BUTTON], 1, animationMove + 20, 10);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[7], Color(1, 1, 1), 1, animationMove + 19, 10);
	}
}
/******************************************************************************/
/*!
\brief
Render credits

\exception None
\return None
*/
/******************************************************************************/
void MainMenu::credits()
{
	RenderUIwithTranparent(meshList[GEO_BACKGROUND2], 10, animationMove, 30);
	RenderTextOnScreen(meshList[GEO_TEXT], my_arr[5], Color(0, 0, 0), 3, animationMove - 10, 45);

	int j = 40;
	for (int i = 15; i <= 18; ++i)
	{
		j -= 5;
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[i], Color(0, 0, 0), 2, animationMove-10, j);
	}

	//////////////////////////
	//     BACK BUTTON     //
	/////////////////////////

	if ((1632 * SCREEN_WIDTH / 1920 > Singleton::getInstance()->mousex && 1248 * SCREEN_WIDTH / 1920 < Singleton::getInstance()->mousex) &&
		(935 * SCREEN_HEIGHT / 1080 > Singleton::getInstance()->mousey && 865 * SCREEN_HEIGHT / 1080 <Singleton::getInstance()->mousey))
	{
		//MOUSE CLICK
		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			animation = false;
			RenderUI(meshList[GEO_BUTTON_HOVER], 1, animationMove + 20, 10);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[7], Color(1, 0, 0), 1, animationMove + 19, 10);
			state = MENU_MAIN;
		}
		//MOUSE HOVER
		else
		{
			RenderUI(meshList[GEO_BUTTON_HOVER], 1, animationMove + 20, 10);
			RenderTextOnScreen(meshList[GEO_TEXT], my_arr[7], Color(1, 0, 0), 1, animationMove + 19, 10);
		}
	}
	//DEFAULT
	else
	{
		RenderUI(meshList[GEO_BUTTON], 1, animationMove + 20, 10);
		RenderTextOnScreen(meshList[GEO_TEXT], my_arr[7], Color(1, 1, 1), 1, animationMove + 19, 10);
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
void MainMenu::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

