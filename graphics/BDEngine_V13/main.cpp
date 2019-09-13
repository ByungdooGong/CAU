#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "Application.h"
//#include "Lecture5.h"
//#include "Lecture10.h"
#include "Final.h"
Application *app;
#define Third_run "movie.wav"
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{	
	float pos[3] = { 0.0f, 0.0f, 0.0f };
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		app->ReleaseApp();
		glfwSetWindowShouldClose(window, true);
	}
	app->KeyInput(key, action, mods);
}

void reSizeCallBack(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	app->MouseInput(xpos, ypos);
}
void mouse_clickCallback(GLFWwindow* window, int button, int action, int mods)
{
	
}
int main(){
	
	PlaySound(TEXT(Third_run), NULL, SND_FILENAME | SND_ASYNC);
	glfwInit();
	//Window Creation
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "20131163_ByungdooGong", glfwGetPrimaryMonitor(), NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	//Init GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		printf("Load fail : GLEW Library \n");
		std::cerr << glewGetErrorString(err) << std::endl;
		getchar();
	}
	else
		printf("Ready for GLEW Library \n");

	//Init Rootine
	//app = new Lecture5();
	//app = new Lecture10();
	app = new Final();
	app->InitApp();

	//KeyCallback
	glfwSetKeyCallback(window, KeyCallback);
	//Window resize callback
	glfwSetWindowSizeCallback(window, reSizeCallBack);
	//mouse move callback
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_clickCallback);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Render loop
	while (!glfwWindowShouldClose(window))
	{
		
		glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		//Todo : Render loop
		//......
		app->UpdateApp();
		app->RenderApp();

		//Swap Buffers
		glfwSwapBuffers(window);

		//Window Event
		glfwPollEvents();
	}
	app->ReleaseApp();
	//glfw: Destroy window
	glfwTerminate();
	return 0;
}