#include "Cube.h"

Cube::Cube(){}
Cube::~Cube(){}
Cube::Cube(const char *vsFile, const char *fsFile){
	InitBuffers(vsFile, fsFile);
}
void Cube::ComputeNormal(int *idxList, float *vList)
{
	for (int i = 0; i < 36; i += 3)
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

void Cube::InitBuffers(const char *vsFile, const char *fsFile)
{
	mShader.InitShader(vsFile, fsFile);
	
	float vertices[] = {
		//Position         //UV
		//Top
		1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		//Bottom
		1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
		//Front
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
		//Back
		1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
		//Left
		-1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
		//Right
		1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f, 0.0f, 1.0f
	};

	int indices[] = {
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23
	};
	
	for (int i = 0; i < 36; i++)
	{
		mNormal.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	ComputeNormal(indices, vertices);

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

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

	glBindBuffer(GL_ARRAY_BUFFER, mNormalVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*mNormal.size(), &mNormal[0], GL_STATIC_DRAW);

	//std::cout << mNormal[8].x << mNormal[8].y << mNormal[8].z;
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(2);

	//unbind Buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
}

void Cube::RenderGeometry(glm::mat4 model, glm::mat4 view, glm::mat4 proj)
{
	glUseProgram(mShader.program);
	
	glm::mat4 modelMat = model*mModelMat;

	unsigned int modelLoc = glGetUniformLocation(mShader.program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

	//View transformation
	unsigned int viewLoc = glGetUniformLocation(mShader.program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//Projection
	unsigned int projLoc = glGetUniformLocation(mShader.program, "proj");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
	
	
	if (mIsTexture == true)
	{
		glUseProgram(mShader.program);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTex);
		GLuint texLoc = glGetUniformLocation(mShader.program, m_pLoc);
		glUniform1i(texLoc, 0);
	}
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}		
void Cube::ReleaseBuffers(){
	
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mEBO);
}
