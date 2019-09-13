#include "TextureMgr.h"



TextureMgr::TextureMgr()
{
}


TextureMgr::~TextureMgr()
{
}


void TextureMgr::LoadTexture(GLuint *tex, const char *filename)
{
	glGenTextures(1, tex);

	// load image
	int width, height, channel;
	FILE* fp = _fsopen(filename, "rb", _SH_DENYNO);
	if (fp == NULL)
	{
		std::cout << "Image File not found ! " << std::endl;
		getchar();
	}
	stbi_set_flip_vertically_on_load(true);

	unsigned char *image = stbi_load_from_file(fp, &width, &height, &channel, 0);
	fclose(fp);

	//Bind Texture
	glBindTexture(GL_TEXTURE_2D, *tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(image);

	stbi_set_flip_vertically_on_load(false);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureMgr::LoadTexture(GLuint *tex, const char *filename, int wrapMode)
{
	glGenTextures(1, tex);

	// load image
	int width, height, channel;
	FILE* fp = _fsopen(filename, "rb", _SH_DENYNO);
	if (fp == NULL)
	{
		std::cout << "Image File not found ! " << std::endl;
		getchar();
	}
	stbi_set_flip_vertically_on_load(true);

	unsigned char *image = stbi_load_from_file(fp, &width, &height, &channel, 0);
	fclose(fp);

	//Bind Texture
	glBindTexture(GL_TEXTURE_2D, *tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	
	if (wrapMode == 0)
	{
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}
	else
	{
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	}
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(image);

	stbi_set_flip_vertically_on_load(false);

	glBindTexture(GL_TEXTURE_2D, 0);
}