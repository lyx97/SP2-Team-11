#pragma once
#include "Vector3.h"
#include <map>
#include "Object.h"
#include "Inventory.h"

using std::map;

class Singleton
{
public:
	static Singleton* getInstance()
	{
		static Singleton instance;

		return &instance;
	}
	enum PROGRAM_STATE
	{
		PROGRAM_MENU,
		PROGRAM_GAME,
		PROGRAM_GAME2,
		PROGRAM_EXIT,

		PROGRAM_TOTAL
	};

	PROGRAM_STATE program_state;
	bool stateCheck;
	bool pause;
	bool buttonText;
	map<Object*, int> objectCount;
	float MOUSE_SPEED;

private:
	Singleton()
	{
		program_state = PROGRAM_MENU;
		stateCheck = false;
	}
};

