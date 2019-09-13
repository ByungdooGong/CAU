#pragma once
//#include "RenderMain.h"
#include "Application.h"
#include "Camera.h"
#include "CubeMap.h"
#include "objModel.h"
#include "TextureMgr.h"
#include "LightMain.h"

using namespace glm;
class Final : public Application
{
public:
	Final();
	~Final();
	void InitApp();
	void UpdateApp();
	void RenderApp();
	void ReleaseApp();
	void KeyInput(int key, int action, int mode);
	void MouseInput(float xpos, float ypos);
public:
	ObjModel obj;
	Camera mCamera;
	LightMain light;
	bool flag;
	glm::mat4 mModel, mView, mProj;
	unsigned int tex;
	bool isAlt;
};

