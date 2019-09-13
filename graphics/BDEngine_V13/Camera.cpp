#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}
void Camera::Initilize(){
	//Init Camera value and Matrix
	cameraPos = glm::vec3(0.0f, 40.0f, 30.0f);
	cameraFront = glm::vec3(0.0f, -0.5f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}