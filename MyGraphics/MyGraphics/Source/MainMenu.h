/****************************************************************************/
/*!
\file MainMenu.h
\author Teo Jia Hao
\par email: kelvinteojiahao\@hotmail.com
\brief
Class to define an MainMenu
*/
/****************************************************************************/
#ifndef MAIN_MENU_H
#define MAIN_MENU_H
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "Scene.h"
#include "Mesh.h"
#include "Mtx44.h"
#include "MatrixStack.h"
#include "Sound.h"
#include "Light.h"
#include "Material.h"

using std::string;
using std::ifstream;
using std::vector;
/****************************************************************************/
/*!
Class MainMenu:
\brief Defines a MainMenu class and its methods
*/
/****************************************************************************/
class MainMenu : public Scene
{
	enum MENU_STATE
	{
		MENU_MAIN = 0,
		MENU_START,
		MENU_INSTRUCTIONS,
		MENU_OPTIONS,
		MENU_CREDITS,
		MENU_EXIT,

		MENU_TOTAL
	};

	enum GEO_MENU
	{
		GEO_BACKGROUND,
		GEO_BACKGROUND2,
		GEO_BUTTON,
		GEO_BUTTON_HOVER,
		GEO_TEXT,

		GEO_TOTAL
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,

		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHTENABLED,
		U_NUMLIGHTS,

		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TOTAL,
	};

public:
	MainMenu();
	~MainMenu();

	void Init();
	void Update(double dt);
	void Render();
	void Exit();

	unsigned m_vertexArrayID;
	Mesh *meshList[GEO_TOTAL];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	MS modelStack, viewStack, projectionStack;
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderUI(Mesh* mesh, float size, float x, float y);
	void RenderUIwithTranparent(Mesh* mesh, float size, float x, float y);
	void textFile(string filename);

	void mainMenu();
	void instruction();
	void options();
	void credits();
	MENU_STATE state;

	bool animation = false;
	float animationMove = 40;

	Sound sound;

	string new_line;
	vector<string>my_arr;
};

#endif
