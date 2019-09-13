#pragma once

#include "TextureMgr.h"
#include "Shader.h"
#include "Camera.h"
#include "Cube.h"
#include "Quad.h"
#include "TextureMgr.h" 
#include <sstream>

class RenderMain 
{
public:
	RenderMain();
	~RenderMain();
public:
	void InitApp();
	void UpdateApp();
	void RenderApp(glm::mat4 mModel, glm::mat4 mView, glm::mat4 mProj);
	void ReleaseApp();
	void SetLight(GLuint program);
	//Texture
	GLuint m_pCubeTex[10];
	TextureMgr mTexMgr;
	
	
	//Light
	std::vector<Geometry *> mCubes, mLights;
	Shader mShader;
	
	//Lights
	
};



