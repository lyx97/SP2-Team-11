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
        PROGRAM_GAME3,
		PROGRAM_EXIT,

		PROGRAM_TOTAL
	};

	PROGRAM_STATE program_state;
	map<Object*, int> objectCount;

	bool stateCheck;
	bool pause;
	bool buttonText;
	bool gotSword = false;
	bool gotGun = false;
	bool swordAniDown = false;
	bool swordAniUp = false;
	bool gunAniDown = false;
	bool gunAniUp = false;

	float MOUSE_SPEED;

	int health = 100;
	int rotateSword = 20;
	int rotateGun = 20;

private:
	Singleton()
	{
		program_state = PROGRAM_MENU;
		stateCheck = false;
	}
};

