#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

class Application
{
public:
	Application();
	~Application();

public:
	virtual void InitApp() = 0;
	virtual void UpdateApp() = 0;
	virtual void RenderApp() = 0;
	virtual void ReleaseApp() = 0;

	virtual void KeyInput(int key, int action, int mode) = 0;
	virtual void MouseInput(float xpos, float ypos) = 0;
};

