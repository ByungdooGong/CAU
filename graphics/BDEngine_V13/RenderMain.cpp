#include "RenderMain.h"

glm::vec3 pointLightPositions[4] = {
	glm::vec3(0.7f, 0.2f, 2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f, 2.0f, -12.0f),
	glm::vec3(0.0f, 0.0f, -3.0f)
};
glm::vec3 pointLightColor[4] = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(1.0f, 0.0f, 1.0f)
};

RenderMain::RenderMain()
{
}


RenderMain::~RenderMain()
{
}

void RenderMain::InitApp()
{	
	
	//Light Geometry
	for (int i = 0; i < 4; i++)
	{
		Geometry *pCube = new Cube(".\\GLSL\\whiteBox.vs", ".\\GLSL\\whiteBox.fs");
		mTexMgr.LoadTexture(&m_pCubeTex[i], ".\\Texture\\wooden.jpg");
		pCube->Translate(pointLightPositions[i]);
		pCube->Scale(0.3f);
		mLights.push_back(pCube);
	}
	//Cubes
	//Cube Position
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};
	for (int i = 0; i < 10; i++)
	{
		Geometry *pCube = new Cube(".\\GLSL\\lightModel.vs", ".\\GLSL\\multiLightModel.fs");
		mTexMgr.LoadTexture(&m_pCubeTex[i], ".\\Texture\\wooden.jpg");
		pCube->SetTexture(m_pCubeTex[i], "albedo");
		pCube->Translate(cubePositions[i]);
		//pCube->Scale(0.5f);
		
		mCubes.push_back(pCube);
	}
	
}
void RenderMain::UpdateApp()
{

}
void RenderMain::RenderApp(glm::mat4 mModel, glm::mat4 mView, glm::mat4 mProj)
{
	
	//Light Render
	GLuint program;
	for (int i = 0; i < mLights.size(); i++)
	{
		program = mLights[i]->mShader.program;

		glUseProgram(program);
		GLuint lightColor = glGetUniformLocation(program, "solid");
		glUniform3fv(lightColor, 1, glm::value_ptr(pointLightColor[i]));
		mLights[i]->RenderGeometry(mModel, mView, mProj);
	}
	//Scene
	for (int i = 0; i < mCubes.size(); i++)
	{
		program = mCubes[i]->mShader.program;
		SetLight(program);
		
		mCubes[i]->RenderGeometry(mModel, mView, mProj);
	}
	
}

void RenderMain::ReleaseApp()
{
	for (int i = 0; i < mCubes.size(); i++)
	{
		mCubes[i]->ReleaseBuffers();
	}
	
}
void RenderMain::SetLight(GLuint program)
{
	glUseProgram(program);

	glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 10.0f);
	GLuint vPos = glGetUniformLocation(program, "viewPos");
	glUniform3fv(vPos, 1, glm::value_ptr(camPos));

	glUseProgram(program);
	//Directional Light
	GLuint direction = glGetUniformLocation(program, "dirLight.direction");
	glUniform3f(direction, 0.0f, -1.0f, 0.0f);

	GLuint dirAmbient = glGetUniformLocation(program, "dirLight.ambient");
	glUniform3f(dirAmbient, 1.0f, 1.0f, 1.0f);
	GLuint dirDiffuse = glGetUniformLocation(program, "dirLight.diffuse");
	glUniform3f(dirDiffuse, 1.0f, 1.0f, 0.0f);
	GLuint dirSpec = glGetUniformLocation(program, "dirLight.specular");
	glUniform3f(dirSpec, 1.0f, 1.0f, 1.0f);
	
	//Point Light
	for (int i = 0; i < 4; i++)
	{
		std::string s;
		std::stringstream pos, ambient, diffuse, specular, constant, linear, quadratic;

		pos << "pLight[" << i << "].position";
		ambient << "pLight[" << i << "].ambient";
		diffuse << "pLight[" << i << "].diffuse";
		specular << "pLight[" << i << "].specular";
		constant << "pLight[" << i << "].constant";
		linear << "pLight[" << i << "].linear";
		quadratic << "pLight[" << i << "].quadratic";

		GLuint pLightPos = glGetUniformLocation(program, pos.str().c_str());
		glUniform3fv(pLightPos, 1, glm::value_ptr(pointLightPositions[i]));

		GLuint amb = glGetUniformLocation(program, ambient.str().c_str());
		glUniform3fv(amb, 1, glm::value_ptr(pointLightColor[i]));

		GLuint diff = glGetUniformLocation(program, diffuse.str().c_str());
		glUniform3fv(diff, 1, glm::value_ptr(pointLightColor[i]));

		GLuint spec = glGetUniformLocation(program, specular.str().c_str());
		glUniform3fv(spec, 1, glm::value_ptr(pointLightColor[i]));

		GLuint cons = glGetUniformLocation(program, constant.str().c_str());
		glUniform1f(cons, 1.0f);

		GLuint lin = glGetUniformLocation(program, linear.str().c_str());
		glUniform1f(lin, 0.09f);

		GLuint quad = glGetUniformLocation(program, quadratic.str().c_str());
		glUniform1f(quad, 0.032f);
	}

}
