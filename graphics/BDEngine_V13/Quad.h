#include "Geometry.h"
#include <iostream>
#include "Shader.h"
#include <vector>

class Quad : public Geometry
{
public:
	Quad();
	~Quad();

	void InitBuffers(const char *vsFile, const char *fsFile);
	void RenderGeometry(glm::mat4 model, glm::mat4 view, glm::mat4 proj);
	void ReleaseBuffers();
	void Quad::ComputeNormal(int *idxList, float *vList);
public:
	GLuint mVAO, mVBO, mEBO;
	//Shader mShader;
	float x = 0, y = 0;
	glm::vec4 nv ;
	float data;
	GLuint mNoramlVBO;
	std::vector<glm::vec3> mNormal;
	
};

