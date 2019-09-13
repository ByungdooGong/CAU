#include "Lecture10.h"


Lecture10::Lecture10()
{
}


Lecture10::~Lecture10()
{
}
void Lecture10::InitApp(){
	rm.InitApp();
	sky.InitBuffers();
	mCamera.Initilize();
	ironman[0].loadModel("Ironman.obj", ".\\GLSL\\Reflection.vs", ".\\GLSL\\Reflection.fs");
	ironman[1].loadModel("Ironman.obj", ".\\GLSL\\Refraction.vs", ".\\GLSL\\Refraction.fs");
	suit.LoadTexture(&tex, "ironman_d.tga");
	ironman[2].loadModel("Ironman.obj", ".\\GLSL\\Reflection.vs", ".\\GLSL\\Reflection.fs");
}
void Lecture10::UpdateApp(){
}
void Lecture10::RenderApp(){
	
	
	mView = lookAt(mCamera.cameraPos, mCamera.cameraPos + mCamera.cameraFront, mCamera.cameraUp);
	mProj = perspective(45.0f, (float)800 / (float)800, 0.1f, 1000.0f);
	rm.RenderApp(mModel, mView, mProj);
	sky.Render(mView, mProj);
	
	if (face==0)
		ironman[0].renderModel(mView, mProj, emptytex);
	else if (face == 1)
		ironman[1].renderModel(mView, mProj, emptytex);
	else
		ironman[2].renderModel(mView, mProj, tex);
	
		

}
void Lecture10::ReleaseApp(){
	rm.ReleaseApp();
	ironman[0].releaseModel();
	ironman[1].releaseModel();
}
void Lecture10::KeyInput(int key, int action, int mode)
{
	float cameraSpeed = 0.5f;
	if (key == GLFW_KEY_W)
	{
		mCamera.cameraPos += cameraSpeed*mCamera.cameraFront;
	}
	if (key == GLFW_KEY_S)
	{
		mCamera.cameraPos -= cameraSpeed*mCamera.cameraFront;
	}
	if (key == GLFW_KEY_A)
	{
		mCamera.cameraPos -= normalize(cross(mCamera.cameraFront, mCamera.cameraUp))*cameraSpeed;
	}
	if (key == GLFW_KEY_D)
	{
		mCamera.cameraPos += normalize(cross(mCamera.cameraFront, mCamera.cameraUp))*cameraSpeed;
	}
	if (mode == GLFW_MOD_ALT && action == GLFW_PRESS)
		isAlt = true;
	else if (action == GLFW_RELEASE)
		isAlt = false;
	//reflecion or refraction or ironman
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		face = (face+1) %3;
	}
	
	
}
void Lecture10::MouseInput(float xpos, float ypos){
	float yaw = -90.f;
	float pitch = 0.0f;
	float lastX = 800.0f / 2.0f;
	float lastY = 800.0f / 2.0f;


	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	if (isAlt == true)
	{
		float sensitivity = 0.01f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(radians(yaw)) * cos(radians(pitch));
		front.y = sin(radians(pitch));
		front.z = sin(radians(yaw)) * cos(radians(pitch));
		mCamera.cameraFront = normalize(front);
	}
}
