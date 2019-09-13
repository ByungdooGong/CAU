#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <vector>



class nCube
{
public:
	nCube();
	~nCube();
	void InitBuffer(const char *vsFile, const char *fsFile);
	void RenderCube(glm::mat4 model, glm::mat4 view, glm::mat4 proj, unsigned int tex, int signal, float spot);
	void ReleaseBuffer();
	//Child Class Method
	void ComputeNormal(int *idxList, float *vList);
public:
	float mAngle;
	Shader mShader;
	GLuint mVAO, mVBO, mEBO;
	GLuint mNoramlVBO;
	int signal = 0;
	std::vector<glm::vec3> mNormal;

};

