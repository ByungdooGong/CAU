#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <vector>
class Geometry
{
public:
	Geometry();
	~Geometry();

	virtual void InitBuffers(const char *vsFile, const char *fsFile) = 0;
	virtual void RenderGeometry(glm::mat4 model, glm::mat4 view, glm::mat4 proj) = 0;
	virtual void ReleaseBuffers() = 0;

	void SetTexture(GLuint tex, const char *loc);
	void Translate(glm::vec3 t);
	void Rotate(float angle, glm::vec3 axis);
	void Scale(glm::vec3 s);
	void Scale(float s);

public:
	glm::mat4 mModelMat;

	Shader mShader;
	//Texture
	bool mIsTexture;
	const char *m_pLoc;
	GLuint mTex;
};

