#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
using namespace std;


class texture
{
public:
	texture();
	~texture();
	void Load(unsigned int *tex, const char* name);
	
	
public:
	
	unsigned int tex1, tex2, tex;
};