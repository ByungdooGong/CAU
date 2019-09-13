#pragma once

#include "Application.h"
#include "TextureMgr.h"
#include "Shader.h"
#include "Camera.h"
#include "Cube.h"
#include "Quad.h"
#include "TextureMgr.h"
#include <time.h>

using namespace glm;
using namespace std;
class Lecture5 : public Application
{
public:
	Lecture5();
	~Lecture5();


public:
	void InitApp();
	void UpdateApp();
	void RenderApp();
	void ReleaseApp();
	void KeyInput(int key, int action, int mode);
	void MouseInput(float xpos, float ypos);
	
	bool isAlt = false;
	int numberOfCube = 12;
	Camera mCamera;
	Cube mCube[12];
	Quad mQuad;
	mat4 mModelMat[12], mViewMat, mProjMat, orimat, tmpmat;
	TextureMgr mTex, mTex2;
	GLuint texture, texture2;
	float ang[12], angbox[12], qwe = 0.1f;
};
