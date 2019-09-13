#include "CubeMap.h"


CubeMap::CubeMap()
{
}


CubeMap::~CubeMap()
{
}
void CubeMap::InitBuffers()
{
	float points[] = {
		-500.0f, 500.0f, -500.0f, //front
		-500.0f, -500.0f, -500.0f,
		500.0f, -500.0f, -500.0f,
		500.0f, -500.0f, -500.0f,
		500.0f, 500.0f, -500.0f,
		-500.0f, 500.0f, -500.0f,

		-500.0f, -500.0f, 500.0f,  //back
		-500.0f, 500.0f, 500.0f,
		500.0f, 500.0f, 500.0f,
		500.0f, 500.0f, 500.0f,
		500.0f, -500.0f, 500.0f,
		-500.0f, -500.0f, 500.0f,

		-500.0f, 500.0f, -500.0f, //top
		500.0f, 500.0f, -500.0f,
		500.0f, 500.0f, 500.0f,
		500.0f, 500.0f, 500.0f,
		-500.0f, 500.0f, 500.0f,
		-500.0f, 500.0f, -500.0f,

		-500.0f, -500.0f, -500.0f, //bottom
		-500.0f, -500.0f, 500.0f,
		500.0f, -500.0f, -500.0f,
		500.0f, -500.0f, -500.0f,
		-500.0f, -500.0f, 500.0f,
		500.0f, -500.0f, 500.0f,
		

		-500.0f, -500.0f, 500.0f, //left
		-500.0f, -500.0f, -500.0f,
		-500.0f, 500.0f, -500.0f,
		-500.0f, 500.0f, -500.0f,
		-500.0f, 500.0f, 500.0f,
		-500.0f, -500.0f, 500.0f,

		500.0f, -500.0f, -500.0f,  //right
		500.0f, -500.0f, 500.0f,
		500.0f, 500.0f, 500.0f,
		500.0f, 500.0f, 500.0f,
		500.0f, 500.0f, -500.0f,
		500.0f - 500.0f, -500.0f,
	};
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &points, GL_STATIC_DRAW);
			
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	mShader.InitShader(".\\GLSL\\Cubemap.vs", ".\\GLSL\\Cubemap.fs");
	CreateCubeMap(".\\Sky\\HornstullsStrand\\posz.jpg", ".\\Sky\\HornstullsStrand\\negz.jpg", ".\\Sky\\HornstullsStrand\\negy.jpg", ".\\Sky\\HornstullsStrand\\posy.jpg", ".\\Sky\\HornstullsStrand\\posx.jpg", ".\\Sky\\HornstullsStrand\\negx.jpg", &mTexCube);

}
void CubeMap::CreateCubeMap(const char* front, const char* back, const char* top, const char* bottom, const char* left, const char* right, GLuint* tex_cube)
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, tex_cube);

	//load each image and copy into a aside of the cube-map texture
	Load_Cube_Map_Side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);
	Load_Cube_Map_Side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
	Load_Cube_Map_Side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
	Load_Cube_Map_Side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
	Load_Cube_Map_Side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
	Load_Cube_Map_Side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);
	//format cube map texture
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
bool CubeMap::Load_Cube_Map_Side(GLuint texture, GLenum side_target, const char* file_name)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	int x, y, n;
	int force_channels = 4;
	unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);
	if (!image_data){
		fprintf(stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}
	//non-power-of-2 dimensions check
	if ((x&(x - 1) != 0 || (y&(y - 1)) != 0)){
		fprintf(stderr, "WARNING: image %s is not power-of-2 dimensions\n", file_name);
	}
	//copy image data into 'target' side of cubemap
	glTexImage2D(side_target, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	free(image_data);
	return true;
}
void CubeMap::Render(glm::mat4 view, glm::mat4 proj)
{
	glUseProgram(mShader.program);

	//View transformation
	glm::mat4 viewCube = glm::mat4(glm::mat3(view));
	GLuint viewLoc = glGetUniformLocation(mShader.program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewCube));

	//Projection
	GLuint projLoc = glGetUniformLocation(mShader.program, "proj");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTexCube);
	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}