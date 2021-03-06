
#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "MainMenu.h"
#include "SP2.h"
#include "SP2Scene2.h"
#include "SP2Scene3.h"

using std::cout;
using std::endl;

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

void Application::MouseMove(double &x, double &y)
{
	glfwGetCursorPos(m_window, &x, &y);
	glfwSetCursorPos(m_window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

void Application::ShowCursor()
{
	glfwSetInputMode(m_window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
}
void Application::HideCursor()
{
	glfwSetInputMode(m_window,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
}

void Application::SetMousePosition(double x, double y)
{
	if (x == 0 && y == 0){
		int sizeX = 0;
		int sizeY = 0;

		glfwGetWindowSize(m_window, &sizeX, &sizeY);

		sizeX = (sizeX / 2) - x;
		sizeY = (sizeY / 2) - y;

		glfwSetCursorPos(m_window, sizeX, sizeY);
	}
	else{
		glfwSetCursorPos(m_window, x, y);
	}
}

Application::Application()
{
}

Application::~Application()
{
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h); //update opengl the new window size
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Le Space Xplorer", NULL, NULL);
	//m_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Computer Graphics Hell Yeah", glfwGetPrimaryMonitor(), NULL);
	glfwSetWindowSizeCallback(m_window, resize_callback);
	
	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	glfwSetKeyCallback(m_window, key_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}
}

void Application::Run()
{
	//Main Loop
	
	Scene *scene = new MainMenu();
    //Scene *scene = new SP2();
    //Scene *scene = new SP2Scene2();
    //Scene *scene = new SP2Scene3();

	scene->Init();

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && Singleton::getInstance()->program_state != Singleton::PROGRAM_EXIT && !IsKeyPressed(VK_ESCAPE))
	{
		scene->Update(m_timer.getElapsedTime());
		scene->Render();
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   
		glfwGetCursorPos(m_window, &Singleton::getInstance()->mousex, &Singleton::getInstance()->mousey);
		if (Singleton::getInstance()->stateCheck) 
		{
			delete scene;
			Singleton::getInstance()->stateCheck = false;

			switch (Singleton::getInstance()->program_state)
			{
			case Singleton::PROGRAM_MENU: scene = new MainMenu();
				break;
			case Singleton::PROGRAM_GAME: scene = new SP2();
				break;
			case Singleton::PROGRAM_GAME2: scene = new SP2Scene2();
				break;
			case Singleton::PROGRAM_GAME2R: scene = new SP2();
				break;
			case Singleton::PROGRAM_GAME3: scene = new SP2Scene3();
                break;
			}
			scene->Init();
		}

	} //Check if the ESC key had been pressed or if the window had been closed
	scene->Exit();
	delete scene;
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}
