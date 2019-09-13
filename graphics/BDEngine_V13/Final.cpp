#include "Final.h"

float yaw = -90.f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0f;
float lastY = 800.0f / 2.0f;

Final::Final()
{
}


Final::~Final()
{
}
void Final::InitApp()
{
	mCamera.Initilize();
	light.InitApp();
	
}
void Final::UpdateApp()
{
}
void Final::RenderApp()
{
	mView = lookAt(mCamera.cameraPos, mCamera.cameraPos + mCamera.cameraFront, mCamera.cameraUp);
	mProj = perspective(45.0f, (float)1920/ (float)1080, 0.1f, 1000.0f);
	light.RenderApp(mModel, mView, mProj);
	float camspeed = 0.02f;
	
	if (mCamera.cameraPos.y >= 10.0f && flag ==false)
	{
		mCamera.cameraPos.y -= camspeed;
		mCamera.cameraPos.z += camspeed;
		mCamera.cameraFront.y += 0.025 * camspeed;
	}
	else
	{
		if (mCamera.cameraPos.z >= 26 && flag==false)
		{
			mCamera.cameraPos.x += camspeed;
			mCamera.cameraPos.z -= camspeed;
			mCamera.cameraFront.x -= 0.025 * camspeed;
			
		}
		else
			flag = true;
	}
	std::cout << flag<< std::endl;
}
void Final::ReleaseApp()
{
	light.ReleaseApp();
}
void Final::KeyInput(int key, int action, int mode)
{
	float cameraSpeed = 2.0f;
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
}
void Final::MouseInput(float xpos, float ypos)
{
	

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	if (isAlt == true)
	{
		float sensitivity = 0.2f;
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