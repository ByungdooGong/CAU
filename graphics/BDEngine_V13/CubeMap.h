#pragma once
#include "Geometry.h"
#include "TextureMgr.h"

class CubeMap
{
public:
	CubeMap();
	~CubeMap();
public:
	void InitBuffers();
	void CreateCubeMap(const char* front, const char* back, const char* top, const char* bottom, const char* left, const char* right, GLuint* tex_cube);
	bool Load_Cube_Map_Side(GLuint texture, GLenum side_target, const char* file_name);
	void Render(glm::mat4 view, glm::mat4 proj);
	GLuint mVAO, mVBO;
	Shader mShader;
	GLuint mTexCube;
};

