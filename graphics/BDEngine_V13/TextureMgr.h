#pragma once

#include <stb_image.h>
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class TextureMgr
{
public:
	TextureMgr();
	~TextureMgr();

public:
	void LoadTexture(GLuint *tex, const char *filename);
	void LoadTexture(GLuint *tex, const char *filename, int wrapMode);
};

