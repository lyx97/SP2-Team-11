
#ifndef APPLICATION_H
#define APPLICATION_H
#define SCREEN_HEIGHT 1080
#define SCREEN_WIDTH 1920

#include "timer.h"

class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static void MouseMove(double &x, double &y);
	static bool IsKeyPressed(unsigned short key);

private:

	//Declare a window object
	StopWatch m_timer;
};

#endif