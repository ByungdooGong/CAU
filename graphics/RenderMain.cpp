#include "RenderMain.h"


RenderMain::RenderMain()
{
}


RenderMain::~RenderMain()
{
}
void RenderMain::InitApp()
{
	//Camera
	lastX = 400.0f;
	lastY = 400.0f;

	m_pCam = new Camera();
	m_pCam->InitCam(0.0f, 0.0f, 20.0f, 0.0f, 1.0f, 0.0f);

	//Model
	m_pCube = new Cube(".\\GLSL\\texCube.vs", ".\\GLSL\\texCube.fs");
	mTexMgr.LoadTexture(&mCubeTex, ".\\Texture\\wooden.jpg");
	m_pCube->SetTexture(mCubeTex, "inTexture");

	//Quad
	m_pQuad = new Quad(".\\GLSL\\texCube.vs", ",.\\GLSL\\texCube.fs");
	mTexMgr.LoadTexture(&mQuadTex, ".\\Texture\\checker.jpg");
	m_pQuad->SetTexture(mQuadTex, "inTexture");

	m_pQuad->Translate(glm::vec3(0.0f, -1.0f, 0.0f));
	m_pQuad->Scale(5.0f);
	m_pQuad->Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	//Mesh
	m_nAnimIndex = 0;
	mTexMgr.LoadTexture(&m_AniTex, ".\\Meshes\\RunBoy\\bo3.jpg");
	for (int i = 1; i <= 32; i++)
	{
		std::string s;
		std::stringstream ss;

		if (i < 10)
			ss << ".\\Meshes\\RunBoy\\RunBoy.000" << i << ".obj";
		else if (i >= 10 && i < 100)
			ss << ".\\Meshes\\RunBoy\\RunBoy.00" << i << ".obj";
		else if (i >= 100)
			ss << ".\\Meshes\\RunBoy\\RunBoy.0" << i << ".obj";

		Geometry *m = new ObjModel(ss.str(), ".\\GLSL\\model.vs", ".\\GLSL\\model.fs");
		m->Scale(0.01f);
		m->SetTexture(m_AniTex, "Albedo");
		mAnimMesh.push_back(m);
			
	}
}
