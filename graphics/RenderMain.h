#pragma once
#include "Application.h"
#include "Geometry.h"
#include "TextureMgr.h"
#include "Quad.h"
#include "Cube.h"
#include "Camera.h"

class RenderMain : public Application
{
public:
	RenderMain();
	~RenderMain();

	void InitApp();
	void UpdateApp();
	void RenderApp();
	void ReleaseApp();

	void KeyInput(int key, int action, int mode);
	void MouseMove(double xpos, double ypos);
	void MouseClick(int button, int action, int modes);
public:
	//Texture
	GLuint mCubeTex;
	GLuint mQuadTex;
	TextureMgr mTexMgr;

	//Scene
	Geometry *m_pCube;
	Geometry *m_pQuad;
	Geometry *m_pCam;

	//Mesh
	Geometry *m_pMesh;
	GLuint mMeshTex;

	//Animated Mesh
	int m_nAnimIndex;
	GLuint m_AniTex;
	std::vector<Geometry *>mAnimMesh;

	//Camera
	float lastX;
	float lastY;

};

