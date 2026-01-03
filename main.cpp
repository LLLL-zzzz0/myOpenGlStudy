#include <iostream>
#include "glframework/core.h"
#include <assert.h>
#include "wrapper/checkError.h"
#include "Application/Application.h"
#include "glframework/shader.h"
#include <memory>
#include "glframework/texture.h"
#include "Application/camera/trackBallcameraControl.h"
#include "Application/camera/gameCameraControl.h"
#include "Application/camera/orthoGraphicCamera.h"
#include "Application/camera/perspectiveCamera.h"
#include "glframework/geometry.h"


GLuint Vao, Program;

std::unique_ptr<Shader> shaderProgram;
std::unique_ptr<Geometry> pGeometry;
std::unique_ptr<Texture> pFirstTexture;
glm::mat4 transform(1.0f);

std::unique_ptr<Camera> pCamera;
std::unique_ptr<CameraControl> pCameraControl;

void windowResizeCallback(int iWidth, int iHeight)
{
	GL_CALL(glViewport(0, 0, iWidth, iHeight));//更新视口大小
	std::cout << "窗体大小更新:" << iWidth << "," << iHeight << std::endl;
}

void keyEventCallback(int iKey, int iAction, int iMods)
{
	pCameraControl->onKeyboardEvent(iKey, iAction, iMods);
}

void mouseButtonCallback(int iButton, int iAction, int iMods)
{
	double dXPos = 0.0;
	double dYPos = 0.0;
	app->getCursorPoition(&dXPos, &dYPos);
	pCameraControl->onMouseEvent(iButton, iAction, dXPos, dYPos);
}

void cursorCallback(double dXPos, double dYPos)
{
	pCameraControl->onCursorEvent(dXPos, dYPos);
}

void scrollCallBack(double dYOffset)
{
	pCameraControl->onScrollEvent(-dYOffset);
}

void prepareShader() 
{
	shaderProgram = std::make_unique<Shader>("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
}

void renderGlDrawElements()
{
	//执行opengl画布操作
	//使用设置的清屏颜色清屏
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	//渲染操作
	//绑定当前的program
	shaderProgram->beginShader();

	shaderProgram->setInt("sampler", 0);
	shaderProgram->setMatrix4x4("transform", transform);
	shaderProgram->setMatrix4x4("viewMarTrix", pCameraControl->getMyCamera()->getViewMatrix());
	shaderProgram->setMatrix4x4("projectionMatrix", pCameraControl->getMyCamera()->getProjectionMatrix());


	//绑定VAO
	GL_CALL(glBindVertexArray(pGeometry->getVao()));

	//绘制  无偏移量
	GL_CALL(glDrawElements(GL_TRIANGLES, pGeometry->getIndicesCount(), GL_UNSIGNED_INT, 0));
	//绘制  带偏移量
	//GL_CALL(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(sizeof(int) *3)));

	//解绑VAO
	GL_CALL(glBindVertexArray(0));

	shaderProgram->endShader();
}

void parpareTexture()
{
	pFirstTexture = std::make_unique<Texture>("assets/textures/wood.png", 0);
}

void prepareCamera()
{
	float fSize = 3.0f;
	pCamera = std::make_unique<PerspectiveCamera>(60.0f, (float)app->getWidth() / (float)app->getHeight(), 0.1f, 1000.0f);
	//pCamera = new OrthoGraphicCamera(-fSize, fSize, fSize, -fSize, -fSize, fSize);

	pCameraControl = std::make_unique<TrackBallCameraControl>();
	pCameraControl->setCamera(std::move(pCamera));
}

void prepareGLState()
{
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glDepthFunc(GL_LESS));
}

void prepareGeometry()
{
	pGeometry.reset(Geometry::createBox(6.0f));
}

int main()
{
	if (!app->Init(800, 600))
	{
		return -1;
	}

	app->setKeyCallback(keyEventCallback);
	app->setResizeCallback(windowResizeCallback);
	app->setMouseCallback(mouseButtonCallback);
	app->setCursorCallback(cursorCallback);
	app->setScrollCallback(scrollCallBack);
	

	GL_CALL(glViewport(0, 0, 800, 600)); //设置视口大小 前2个参数是视口左下角位置，后2个参数是视口宽高
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f)); //设置清屏颜色


	prepareShader();
	parpareTexture();
	prepareCamera();
	prepareGeometry();
	prepareGLState();

	//执行窗体循环
	while (app->Update())
	{
		pCameraControl->update();
		renderGlDrawElements();
	}

	pFirstTexture.reset();
	app->Destroy();
	
	return 0;
}