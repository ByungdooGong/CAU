#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Shader.h"

class Triangle
{
public:
	Triangle();
	~Triangle();

	void InitBuffers(const char *vsFile, const char *fsFile);
	void RenderTriangle();
	void ReleaseBuffers();
	

public:
	GLuint mVAO, mVBO;
	Shader mShader;
	float mVertices[9];
	float vertices[9];
	float rcol = 1.0f, bcol = 0.0f, gcol = 1.0f;
	float x, y, z;
};