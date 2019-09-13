#pragma once
#include "RenderMain.h"
#include "Application.h"
#include "Camera.h"
#include "CubeMap.h"
#include "objModel.h"
#include "TextureMgr.h"

using namespace glm;
class Lecture10 : public Application
{
public:
	Lecture10();
	~Lecture10();
public:
	void InitApp();
	void UpdateApp();
	void RenderApp();
	void ReleaseApp();
	void KeyInput(int key, int action, int mode);
	void MouseInput(float xpos, float ypos);

	TextureMgr suit;
	ObjModel ironman[3];
	CubeMap sky;
	RenderMain rm;
	Camera mCamera;
	bool isAlt = false;
	glm::mat4 mModel, mView, mProj;
	int face = 0;
	unsigned int emptytex, tex;
};

