#include "Quad.h"

Quad::Quad()
{
}


Quad::~Quad()
{
}
void Quad::ComputeNormal(int *idxList, float *vList)
{
	for (int i = 0; i < 6; i += 3)
	{
		int v1Idx = idxList[i] * 5;
		glm::vec3 v1 = glm::vec3(vList[v1Idx], vList[v1Idx + 1], vList[v1Idx + 2]);

		int v2Idx = idxList[i + 1] * 5;
		glm::vec3 v2 = glm::vec3(vList[v2Idx], vList[v2Idx + 1], vList[v2Idx + 2]);

		int v3Idx = idxList[i + 2] * 5;
		glm::vec3 v3 = glm::vec3(vList[v3Idx], vList[v3Idx + 1], vList[v3Idx + 2]);

		glm::vec3 v1_v2 = v1 - v2;
		glm::vec3 v1_v3 = v1 - v3;

		glm::vec3 cVec = glm::cross(v1_v2, v1_v3);

		mNormal[idxList[i]] = glm::normalize(cVec);
		mNormal[idxList[i + 1]] = glm::normalize(cVec);
		mNormal[idxList[i + 2]] = glm::normalize(cVec);
	}
}
void Quad::InitBuffers(const char *vsFile, const char *fsFile)
{
	mShader.InitShader(vsFile, fsFile);

	float vertices[] = {
		// positions      //texture coords
		-80.0f, 4.0f, -80.0f, 0.0f, 0.0f,
		80.0f, 4.0f, -80.0f, 1.0f, 0.0f,
		80.0f, 4.0f, 80.0f, 1.0f, 1.0f,
		-80.0f, 4.0f, 80.0f, 0.0f, 1.0f
	};

	int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	for (int i = 0; i < 4; i++)
	{
		mNormal.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	ComputeNormal(indices, vertices);

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);
	glGenBuffers(1, &mNoramlVBO);

	//Bind Buffers
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, mNoramlVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*mNormal.size(), &mNormal[0], GL_STATIC_DRAW);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(2);

	//unbind Buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Quad::RenderGeometry(glm::mat4 model, glm::mat4 view, glm::mat4 proj){
	
	glUseProgram(mShader.program);
	unsigned int modelLoc = glGetUniformLocation(mShader.program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//View transformation
	unsigned int viewLoc = glGetUniformLocation(mShader.program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//Projection
	unsigned int projLoc = glGetUniformLocation(mShader.program, "proj");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

	GLuint vPos = glGetUniformLocation(mShader.program, "vPos");
	glUniform3f(vPos, 1.0f, 0.0f, 0.0f);

	GLuint lightPosition = glGetUniformLocation(mShader.program, "lightPosition");
	glUniform3f(lightPosition, 0.0f, 10.0f, 10.0f);

	GLuint lightColor = glGetUniformLocation(mShader.program, "lightColor");
	glUniform3f(lightColor, 1.0f, 0.0f, 0.0f);

	if (mIsTexture == true)
	{
		glUseProgram(mShader.program);
		glActiveTexture(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mTex);
		GLuint texLoc = glGetUniformLocation(mShader.program, m_pLoc);
		glUniform1i(texLoc, 0);
	}
	/*
	GLint lkind = glGetUniformLocation(mShader.program, "sign");
	glUniform1i(lkind, signal);

	//Spot Light Setting
	GLuint spotDir = glGetUniformLocation(mShader.program, "spotDir");
	glUniform3f(spotDir, 0.0f, -1.0f, -1.0f);

	GLuint cutoff = glGetUniformLocation(mShader.program, "cutOff");
	glUniform1f(cutoff, glm::cos(spot));
	*/		
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Quad::ReleaseBuffers(){
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mEBO);
}
