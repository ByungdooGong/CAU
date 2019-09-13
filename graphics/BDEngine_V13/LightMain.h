#pragma once

//#include "Geometry.h"
#include "objModel.h"
#include "CubeMap.h"
#include "Cube.h"
#include "Quad.h"
#include "TextureMgr.h"
class LightMain
{
public:
	LightMain();
	~LightMain();
public:
	void InitApp();
	void RenderApp(glm::mat4 mModel, glm::mat4 mView, glm::mat4 mProj);
	void ReleaseApp();
	void SetLight(GLuint program);

	//ObjModel obj, spot, headlight[4];
	//Quad back;
	//TextureMgr ct;
	CubeMap sky;
	std::vector<ObjModel *> ob, lightLine, lamp;
	std::vector<Geometry *> mLights;
	unsigned int tex, ctex, stex;
	float ceta = 180.0f, delta = 90.0f, alpha = 0;
};

