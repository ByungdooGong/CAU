#pragma once

//#include "texture.h"
#include "vertexBufferObject.h"
#include <string.h>
#include <sstream>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Geometry.h"

#define ESZ(elem) (int)elem.size()
#define RFOR(q,n) for(int q=n;q>=0;q--)
/********************************

Class:	CObjModel

Purpose: Class for handling obj
		 model files.

********************************/

class ObjModel 
{
public:
	bool loadModel(std::string sFileName, const char* vsFile, const char* fsFile);
	void renderModel(glm::mat4 view, glm::mat4 proj, unsigned int tex);
	void releaseModel();

	int getPolygonCount();
	
	void translate(glm::vec3 t);
	void rotation(glm::vec3 axis, float angle);
	void scale(glm::vec3 s);

	GLuint getProgram();

	ObjModel();
	ObjModel(std::string sFileName, const char* vsFile, const char* fsFile);
	Shader mShader;
private:
	bool bLoaded;
	int iAttrBitField;
	int iNumFaces;
	
	glm::mat4 mModel;
	

	CVertexBufferObject vboModelData;
	GLuint uiVAO;
//	CTexture tAmbientTexture;
};