#include "LightMain.h"


LightMain::LightMain()
{
}
LightMain::~LightMain()
{
}
glm::vec3 spotLightPosition[] = {
	glm::vec3(5.0f, 10.0f, 10.0f),
	glm::vec3(26.0f, 14.0f, 15.0f),
	glm::vec3(-20.0f, 20.0f, 7.0f),
	glm::vec3(-8.0f, 13.0f, -13.0f),
	glm::vec3(-8.0f, 5.0f, 20.0f)
};
glm::vec3 spotLightColor[] = {
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 0.0f)
};


void LightMain::InitApp()
{
	//Light Geometry
	for (int i = 0; i < 5; i++)
	{
		Geometry *pCube = new Cube(".\\GLSL\\whiteBox.vs", ".\\GLSL\\whiteBox.fs");
		pCube->Translate(spotLightPosition[i]);
		mLights.push_back(pCube);
	}
	ObjModel *pOb = new ObjModel(".\\Meshes\\new_20th.obj", ".\\GLSL\\century.vs", ".\\GLSL\\century.fs");
	ob.push_back(pOb);
		
	for (int i = 0; i < 5; i++)
	{
		ObjModel *pLL = new ObjModel(".\\Meshes\\new.obj", ".\\GLSL\\sample.vs", ".\\GLSL\\sample.fs");
		pLL->translate(spotLightPosition[i]);
		lightLine.push_back(pLL);

		ObjModel *pL = new ObjModel(".\\Meshes\\lamp.obj", ".\\GLSL\\lamp.vs", ".\\GLSL\\lamp.fs");
		pL->translate(spotLightPosition[i]);
		lamp.push_back(pL);
	}
	lamp[1]->rotation(glm::vec3(0.0f, 0.0f, 1.0f), 90.0f);
	lightLine[1]->rotation(glm::vec3(0.0f, 0.0f, 1.0f), 90.0f);
	lamp[2]->rotation(glm::vec3(0.0f, 0.0f, 1.0f), -90.0f);
	lightLine[2]->rotation(glm::vec3(0.0f, 0.0f, 1.0f), -90.0f);
	lamp[3]->rotation(glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
	lightLine[3]->rotation(glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);

}
void LightMain::RenderApp(glm::mat4 mModel, glm::mat4 mView, glm::mat4 mProj)
{
	GLuint program;
	/*for (int i = 0; i < mLights.size(); i++)
	{
		program = mLights[i]->mShader.program;

		glUseProgram(program);
		GLuint lightColor = glGetUniformLocation(program, "solid");
		glUniform3fv(lightColor, 1, glm::value_ptr(spotLightColor[i]));
		mLights[i]->RenderGeometry(mModel, mView, mProj);
	}*/
	

	ob[0]->renderModel(mView, mProj, tex);
	SetLight(ob[0]->mShader.program);

	for (int i = 1; i < mLights.size(); i++)
	{
		lightLine[i]->renderModel(mView, mProj, tex);
		lamp[i]->renderModel(mView, mProj, tex);
	}
	
	if (ceta <= 90)
		ceta = 90.0f;
	else
	{
		ceta -= 0.01f;
		lightLine[1]->rotation(glm::vec3(0.0f, 0.0f, 1.0f), -0.01f);
		lamp[1]->rotation(glm::vec3(0.0f, 0.0f, 1.0f), -0.01f);
		
	}
	
	if (delta <= 0)
		delta = 0.0f;
	else
	{
		delta -= 0.01f;
		lightLine[2]->rotation(glm::vec3(0.0f, 0.0f, 1.0f), 0.01f);
		lamp[2]->rotation(glm::vec3(0.0f, 0.0f, 1.0f), 0.01f);
	}
	if (alpha >= 90)
		alpha = 90.0f;
	else
	{
		alpha += 0.01f;
		lightLine[3]->rotation(glm::vec3(1.0f, 0.0f, 0.0f), -0.01f);
		lamp[3]->rotation(glm::vec3(1.0f, 0.0f, 0.0f), -0.01f);
	}
	
}
void LightMain::ReleaseApp()
{
	ob[0]->releaseModel();
	for (int i = 0; i < 5; i++)
	{
		mLights[i]->ReleaseBuffers();
		lightLine[i]->releaseModel();
		lamp[i]->releaseModel();
	}
	
}
void LightMain::SetLight(GLuint program)
{
	glm::vec3 spotLightDir[] = {
		glm::vec3(0.0f, 1.0f, -1.0f),
		glm::vec3(glm::cos(glm::radians(ceta)), glm::sin(glm::radians(ceta)), -1.0f),
		glm::vec3(glm::sin(glm::radians(delta)), glm::cos(glm::radians(delta)), -0.5f),
		glm::vec3(0.5f, glm::sin(glm::radians(alpha)), glm::cos(glm::radians(alpha))),
		
	};
	glUseProgram(program);

	glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 100.0f);
	GLuint vPos = glGetUniformLocation(program, "viewPos");
	glUniform3fv(vPos, 1, glm::value_ptr(camPos));

	glUseProgram(program);
	//Directional Light
	GLuint direction = glGetUniformLocation(program, "dirLight.direction");
	glUniform3f(direction, 0.0f, -1.0f, 0.0f);

	GLuint dirAmbient = glGetUniformLocation(program, "dirLight.ambient");
	glUniform3f(dirAmbient, 1.0f, 1.0f, 1.0f);
	GLuint dirDiffuse = glGetUniformLocation(program, "dirLight.diffuse");
	glUniform3f(dirDiffuse, 1.0f, 1.0f, 1.0f);
	GLuint dirSpec = glGetUniformLocation(program, "dirLight.specular");
	glUniform3f(dirSpec, 1.0f, 1.0f, 1.0f);
	for (int i = 0; i < 5; i++)
	{
		std::string s;
		std::stringstream pos, ambient, diffuse, specular, constant, linear, quadratic, spot;

		pos << "pLight[" << i << "].position";
		ambient << "pLight[" << i << "].ambient";
		diffuse << "pLight[" << i << "].diffuse";
		specular << "pLight[" << i << "].specular";
		constant << "pLight[" << i << "].constant";
		linear << "pLight[" << i << "].linear";
		quadratic << "pLight[" << i << "].quadratic";
		spot << "spotDir[" << i << "]";
		//Spot Light Setting
		GLuint spotDir = glGetUniformLocation(program, spot.str().c_str());
		glUniform3fv(spotDir, 1, glm::value_ptr(spotLightDir[i]));

		GLuint cutoff = glGetUniformLocation(program, "cutOff");
		glUniform1f(cutoff, glm::cos(100.0f));
		//Point Light Setting
		GLuint pLightPos = glGetUniformLocation(program, pos.str().c_str());
		glUniform3fv(pLightPos, 1, glm::value_ptr(spotLightPosition[i]));

		GLuint amb = glGetUniformLocation(program, ambient.str().c_str());
		glUniform3fv(amb, 1, glm::value_ptr(spotLightColor[i]));

		GLuint diff = glGetUniformLocation(program, diffuse.str().c_str());
		glUniform3fv(diff, 1, glm::value_ptr(spotLightColor[i]));

		GLuint spec = glGetUniformLocation(program, specular.str().c_str());
		glUniform3fv(spec, 1, glm::value_ptr(spotLightColor[i]));

		GLuint cons = glGetUniformLocation(program, constant.str().c_str());
		glUniform1f(cons, 1.0f);

		GLuint lin = glGetUniformLocation(program, linear.str().c_str());
		glUniform1f(lin, 0.019f);

		GLuint quad = glGetUniformLocation(program, quadratic.str().c_str());
		glUniform1f(quad, 0.0132f);
	}
}