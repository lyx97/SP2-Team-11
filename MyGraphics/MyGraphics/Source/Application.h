
#ifndef APPLICATION_H
#define APPLICATION_H
#define SCREEN_HEIGHT 1024
#define SCREEN_WIDTH 1280
#define GRAVITY 5.f

#include "timer.h"
#include "Singleton.h"

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
	static void SetMousePosition(double x = 0, double y = 0);
	static void ShowCursor();
	static void HideCursor();
private:

	//Declare a window object
	StopWatch m_timer;
};

#endif