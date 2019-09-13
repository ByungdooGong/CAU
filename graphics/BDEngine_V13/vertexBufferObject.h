#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <vector>
/********************************

Class:	CVertexBufferObject

Purpose:	Wraps OpenGL vertex buffer
			object.

********************************/

class CVertexBufferObject
{
public:
	void createVBO(int a_iSize = 0);
	void releaseVBO();

	void* mapBufferToMemory(int iUsageHint);
	void* mapSubBufferToMemory(int iUsageHint, GLuint uiOffset, GLuint uiLength);
	void unmapBuffer();

	void bindVBO(int a_iBufferType = GL_ARRAY_BUFFER);
	void uploadDataToGPU(int iUsageHint);
	
	void addData(void* ptrData, GLuint uiDataSize);

	void* getDataPointer();
	GLuint getBuffer();

	CVertexBufferObject();

private:
	GLuint uiBuffer;
	int iSize;
	int iBufferType;
	std::vector<GLbyte> data;

	bool bDataUploaded;
};