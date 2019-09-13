#include "Lecture5.h"


Lecture5::Lecture5()
{
}


Lecture5::~Lecture5()
{
}
void Lecture5::InitApp()
{
	srand((unsigned)time(NULL));
	//Init Shader 
	float initPos[3] = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < numberOfCube; i++)
	{
		mCube[i].InitBuffers(".\\GLSL\\Cube.vs", ".\\GLSL\\Cube.fs");
	}
	mQuad.InitBuffers(".\\GLSL\\Quad.vs", ".\\GLSL\\Quad.fs");
	//Tex Params
	//Init Texture unit

	mTex.LoadTexture(&texture, ".\\Texture\\wooden.jpg");
	mTex2.LoadTexture(&texture2, ".\\Texture\\checker.jpg");
	//Init Tex params
	for (int i = 0; i < numberOfCube; i++)
	{
		glUseProgram(mCube[i].mShader.program);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		GLint tex1Loc = glGetUniformLocation(mCube[i].mShader.program, "inTexture");
		glUniform1i(tex1Loc, 0);

	}
	glUseProgram(mQuad.mShader.program);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	GLint tex2Loc = glGetUniformLocation(mQuad.mShader.program, "inTexture");
	glUniform1i(tex2Loc, 1);

	for (int i = 0; i < numberOfCube; i++)
	{
		ang[i] = ((float)rand() / (float)RAND_MAX);
		angbox[i] = 0;
	}

	mCamera.Initilize();
}
void Lecture5::UpdateApp()
{

}
void Lecture5::RenderApp()
{
	
	for (int i = 0; i<12; i++)
		angbox[i] += ang[i];
	for (int i = 0; i < numberOfCube; i++)
		glUseProgram(mCube[i].mShader.program);
	glUseProgram(mQuad.mShader.program);
	mViewMat = lookAt(mCamera.cameraPos, mCamera.cameraPos + mCamera.cameraFront, mCamera.cameraUp);
	mProjMat = perspective(45.0f, (float)800 / (float)800, 0.1f, 1000.0f);

	glm::mat4 world;
	float mWorldAngle = 0.01f;
	world = glm::rotate(world, mWorldAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	tmpmat = rotate(tmpmat, 0.01f, vec3(0.0f, 0.1f, 0.0f));

	orimat = rotate(orimat, 0.01f, vec3(0.0f, 0.1f, 0.0f));
	mModelMat[0] = translate(orimat, vec3(-6.0f, 0.0f, -3.0f));
	mModelMat[1] = translate(orimat, vec3(-2.0f, 0.0f, -3.0f));
	mModelMat[2] = translate(orimat, vec3(2.0f, 0.0f, -3.0f));
	mModelMat[3] = translate(orimat, vec3(6.0f, 0.0f, -3.0f));
	mModelMat[4] = translate(orimat, vec3(-6.0f, 0.0f, 0.0f));
	mModelMat[5] = translate(orimat, vec3(-2.0f, 0.0f, 0.0f));
	mModelMat[6] = translate(orimat, vec3(2.0f, 0.0f, 0.0f));
	mModelMat[7] = translate(orimat, vec3(6.0f, 0.0f, 0.0f));
	mModelMat[8] = translate(orimat, vec3(-6.0f, 0.0f, 3.0f));
	mModelMat[9] = translate(orimat, vec3(-2.0f, 0.0f, 3.0f));
	mModelMat[10] = translate(orimat, vec3(2.0f, 0.0f, 3.0f));
	mModelMat[11] = translate(orimat, vec3(6.0f, 0.0f, 3.0f));

	for (int i = 0; i < 12; i++)
		mModelMat[i] = rotate(mModelMat[i], angbox[i], vec3(0.0f, 0.1f, 0.0f));

	mQuad.RenderGeometry(tmpmat, mViewMat, mProjMat);


	for (int i = 0; i < numberOfCube; i++)
	{
		mCube[i].RenderGeometry(mModelMat[i], mViewMat, mProjMat);
	}
}
void Lecture5::ReleaseApp()
{
}
void Lecture5::KeyInput(int key, int action, int mode)
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
}
void Lecture5::MouseInput(float xpos, float ypos)
{
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
		float sensitivity = 0.05f;
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