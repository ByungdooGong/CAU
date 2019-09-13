

#pragma once

#include <iostream>
#include <GL/glew.h>


class Shader
{
public:
	GLuint program;
	GLuint vaoHandle;
	GLuint vboHandles[2];

public:
	Shader(void);
	~Shader(void);

public:
	char* readShaderSource(const char* shaderFile);
	void InitShader(const char* vShaderFile, const char* fShaderFile);
};

