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
#include "glframework/light.h"
#include "glframework/material.h"

std::unique_ptr<Shader> shaderProgram;
std::unique_ptr<Geometry> pGeometry;
std::shared_ptr<Texture> pDiffuseTexture;
std::shared_ptr<Texture> pSpecularTexture;

glm::mat4 transform(1.0f);

std::unique_ptr<Camera> pCamera;
std::unique_ptr<CameraControl> pCameraControl;

// 新增：光照和材质
std::unique_ptr<Light> pLight;
std::unique_ptr<Material> pMaterial;

glm::vec3 vec3LightPos(7.0f, 7.0f, 10.0f);
glm::vec3 vec3LightColor(0.5f, 0.2f, 0.2f);

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
	shaderProgram = std::make_unique<Shader>("assets/shaders/lighting.vert", "assets/shaders/lighting.frag");
}

void renderGlDrawElements()
{
	//执行opengl画布操作
	//使用设置的清屏颜色清屏
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	//渲染操作
	//绑定当前的program
	shaderProgram->beginShader();

	//shaderProgram->setInt("sampler", 0);
	//shaderProgram->setVec3("lightPos", vec3LightPos);
	//shaderProgram->setVec3("lightColor", vec3LightColor);
	shaderProgram->setMatrix4x4("model", transform);
	shaderProgram->setMatrix4x4("view", pCameraControl->getMyCamera()->getViewMatrix());
	shaderProgram->setMatrix4x4("projection", pCameraControl->getMyCamera()->getProjectionMatrix());

	// 设置相机位置（用于光照计算）
	shaderProgram->setVec3("viewPos", pCameraControl->getMyCamera()->m_vec3Position);

	// 应用光照
	if (pLight) 
	{
		pLight->applyToShader(*shaderProgram, "light");
	}

	// 应用材质
	if (pMaterial) 
	{
		pMaterial->applyToShader(*shaderProgram);
	}

	//绑定VAO
	GL_CALL(glBindVertexArray(pGeometry->getVao()));

	//绘制  无偏移量
	GL_CALL(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0));
	//绘制  带偏移量
	//GL_CALL(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(sizeof(int) *3)));

	//解绑VAO
	GL_CALL(glBindVertexArray(0));

	shaderProgram->endShader();
}

void parpareTexture()
{
	pDiffuseTexture = std::make_shared<Texture>("assets/textures/wood.png", 0);
}

void prepareCamera()
{
	float fSize = 3.0f;
	pCamera = std::make_unique<PerspectiveCamera>(60.0f, (float)app->getWidth() / (float)app->getHeight(), 0.1f, 1000.0f);
	//pCamera = new OrthoGraphicCamera(-fSize, fSize, fSize, -fSize, -fSize, fSize);

	pCameraControl = std::make_unique<TrackBallCameraControl>();
	pCameraControl->setCamera(std::move(pCamera));
}

void prepareLight()
{
	pLight = std::make_unique<Light>(Light::DIRECTIONALLIGHT);
	pLight->m_vec3Driection = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.5f));
	pLight->m_vec3Ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	pLight->m_vec3Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	pLight->m_vec3specular = glm::vec3(1.0f, 1.0f, 1.0f);
}

void prepareMaterial()
{
	//// 创建默认材质
	//pMaterial = std::make_unique<Material>();

	//// 设置纹理
	//pMaterial->diffuseTexture = pDiffuseTexture;

	pMaterial.reset(Material::createPlastic());
}

void prepareGLState()
{
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glDepthFunc(GL_LESS));

	// 启用背面剔除
	GL_CALL(glEnable(GL_CULL_FACE));
	GL_CALL(glCullFace(GL_BACK));
}

void prepareGeometry()
{
	//pGeometry.reset(Geometry::createSphere(6.0f));
	pGeometry.reset(Geometry::createBox(3.0f));
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
	prepareLight();
	prepareMaterial();
	prepareGLState();

	//执行窗体循环
	while (app->Update())
	{
		pCameraControl->update();
		renderGlDrawElements();
	}

	pMaterial.reset();

	// 2. 释放纹理
	pDiffuseTexture.reset();
	pSpecularTexture.reset();

	// 3. 释放几何体（会调用glDeleteBuffers等）
	pGeometry.reset();

	// 4. 释放着色器
	shaderProgram.reset();

	// 5. 释放相机和控制器
	pCameraControl.reset();
	pCamera.reset(); // 如果pCamera已经从pCameraControl转移

	app->Destroy();
	
	return 0;
}