
#ifndef APPLICATION_H
#define APPLICATION_H
#define SCREEN_HEIGHT 1080 / 2
#define SCREEN_WIDTH 1920 / 2
#define GRAVITY 9.8f / 16.f

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