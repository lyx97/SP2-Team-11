#pragma once
#include "Vector3.h"
#include <map>
#include <vector>
#include "Object.h"
#include "Inventory.h"
#include "Bullet.h"

using std::map;
using std::vector;

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
	bool handDown = false;
	bool handUp = false;
	bool fistDown = false;
	bool fistUp = false;

	float MOUSE_SPEED;
	float rotateHand = 45.f;
	float moveFist = 10.f;

	int health = 100;
	int rotateSword = 20;
	int rotateGun = 20;
	int oreCount;

	double mousex;
	double mousey;

private:
	Singleton()
	{
		program_state = PROGRAM_GAME;
		stateCheck = false;

		mousex = 400;
		mousey = 300;
	}
};

