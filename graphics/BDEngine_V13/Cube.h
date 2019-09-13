#include "Geometry.h"
#include <sstream>
#include <vector>

class Cube : public Geometry
{
public:
	Cube();
	~Cube();
	Cube(const char *vsFile, const char *fsFile);

	void InitBuffers(const char *vsFile, const char *fsFile);
	void RenderGeometry(glm::mat4 model, glm::mat4 view, glm::mat4 proj);
	void ReleaseBuffers();
	//void SetLight(GLuint program);

	//Child Class Method
	void ComputeNormal(int *idxList, float *vList);
public:
	GLuint mVAO, mVBO, mEBO;
	GLuint mNormalVBO;

	std::vector<glm::vec3> mNormal;
	//Shader mShader;
	
};

