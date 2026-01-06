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
#include "glframework/light/directionalLight.h"
#include "glframework/light/ambientLight.h"
#include "glframework/material/phongMaterial.h"
#include "glframework/material/whiteMaterial.h"
#include "glframework/mesh.h"
#include "glframework/renderer/renderer.h"
#include "glframework/light/pointLight.h"
#include "glframework/light/spotLight.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

std::shared_ptr<Camera> pCamera;
std::unique_ptr<CameraControl> pCameraControl;

std::unique_ptr<Renderer> pRenderer;
std::vector<std::shared_ptr<Mesh>> vctMesh;
std::shared_ptr<DirectionalLight> pDirectionalLight;
std::shared_ptr<AmbientLight> pAmbientLight;
std::shared_ptr<PointLight> pPointLight;
std::shared_ptr<SpotLight> pSpotLight;

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

void prepareCamera()
{
	float fSize = 3.0f;
	pCamera = std::make_shared<PerspectiveCamera>(60.0f, (float)app->getWidth() / (float)app->getHeight(), 0.1f, 1000.0f);
	//pCamera = new OrthoGraphicCamera(-fSize, fSize, fSize, -fSize, -fSize, fSize);

	pCameraControl = std::make_unique<TrackBallCameraControl>();
	pCameraControl->setCamera(pCamera);
}

void prepare()
{
	pRenderer = std::make_unique<Renderer>();

	//创建geometry
	std::shared_ptr<Geometry> pGeometry(Geometry::createBox(4.0f));
	//创建material
	std::shared_ptr<PhongMaterial> pMaterial = std::make_shared<PhongMaterial>();
	std::shared_ptr<Texture> pDiffuseTexture = std::make_shared<Texture>("assets/textures/box.png", 0);
	std::shared_ptr<Texture> pSpecularMaskTexture = std::make_shared<Texture>("assets/textures/sp_mask.png", 1);
	pMaterial->setShiness(16.0f);
	pMaterial->setDiffuseTexture(pDiffuseTexture);
	pMaterial->setSpecularMaskTexture(pSpecularMaskTexture);

	std::shared_ptr<Geometry> pWhiteLight(Geometry::createSphere(0.5f));
	std::shared_ptr<WhiteMaterial> pWhiteMaterial = std::make_shared<WhiteMaterial>();

	//生成mesh
	std::shared_ptr<Mesh> pMesh = Mesh::Create(pGeometry, pMaterial);
	std::shared_ptr<Mesh> pWhiteMesh = Mesh::Create(pWhiteLight, pWhiteMaterial);

	pWhiteMesh->setPosition(glm::vec3(3.0f, 0.5f, 0.5f));
	pWhiteMesh->setParent(pMesh);

	vctMesh.push_back(pMesh);
	vctMesh.push_back(pWhiteMesh);

	pPointLight = PointLight::Create();
	pPointLight->setPosition(pWhiteMesh->getPosition());
	pPointLight->setK2(0.017f);
	pPointLight->setK1(0.07f);
	pPointLight->setKc(1.0f);

	pSpotLight = SpotLight::Create();
	pSpotLight->setPosition(pWhiteMesh->getPosition());
	pSpotLight->setTargetDirection(glm::vec3(-1.0f, 0.0f, 0.0f));
	pSpotLight->setInnerAngle(30.0f);
	pSpotLight->setOuterAngle(60.0f);

	pDirectionalLight = std::make_shared<DirectionalLight>();
	pAmbientLight = std::make_shared<AmbientLight>();
	pAmbientLight->setColor(glm::vec3(0.1f));
}

void initIMGUI()
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	//设置ImGui与GLFW和Opengl的绑定
	ImGui_ImplGlfw_InitForOpenGL(app->getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
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

	prepareCamera();
	prepare();

	//执行窗体循环
	while (app->Update())
	{
		pCameraControl->update();
		//pRenderer->render(vctMesh, pCamera, pDirectionalLight, pAmbientLight);
		//pRenderer->render(vctMesh, pCamera, pPointLight, pAmbientLight);
		pRenderer->render(vctMesh, pCamera, pSpotLight, pAmbientLight);
	}

	// 5. 释放相机和控制器
	pCameraControl.reset();
	pCamera.reset(); // 如果pCamera已经从pCameraControl转移

	pDirectionalLight.reset();
	pPointLight.reset();
	pSpotLight.reset();
	pAmbientLight.reset();
	pRenderer.reset();
	for (int i = 0; i < vctMesh.size(); i++)
	{
		vctMesh[i].reset();
	}

	app->Destroy();
	
	return 0;
}