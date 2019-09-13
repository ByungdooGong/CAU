#include "Geometry.h"


Geometry::Geometry()
{
	mIsTexture = false;
}


Geometry::~Geometry()
{
}
void Geometry::Translate(glm::vec3 t)
{
	mModelMat = glm::translate(mModelMat, t);
}
void Geometry::Rotate(float angle, glm::vec3 axis)
{
	mModelMat = glm::rotate(mModelMat, angle, axis);
}
void Geometry::Scale(glm::vec3 s)
{
	mModelMat = glm::scale(mModelMat, s);
}
void Geometry::Scale(float s)
{
	mModelMat = glm::scale(mModelMat, glm::vec3(s, s, s));
}
void Geometry::SetTexture(GLuint tex, const char *loc)
{
	mIsTexture = true;
	m_pLoc = loc;
	mTex = tex;

}