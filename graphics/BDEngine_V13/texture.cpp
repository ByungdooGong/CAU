#include "texture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

texture::texture(){}
texture::~texture(){}
void texture::Load(unsigned int *tex, const char* name){
	glGenTextures(1, tex);

	int width, height, channel;
	FILE* fp = _fsopen(name, "rb", _SH_DENYNO);
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
	//set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(image);

	//stbi_set_flip_vertically_on_load(false);
}





