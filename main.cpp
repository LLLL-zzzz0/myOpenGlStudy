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
#include "Application/assimpLoder.h"
#include "glframework/node.h"

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
std::shared_ptr<Scene> pScene;
std::shared_ptr<Node> pNode;
glm::vec3 clearColor(0.6f,0.6f,0.6f);

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
	appInstance->getCursorPoition(&dXPos, &dYPos);
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
	pCamera = std::make_shared<PerspectiveCamera>(60.0f, (float)appInstance->getWidth() / (float)appInstance->getHeight(), 0.1f, 1000.0f);
	//pCamera = new OrthoGraphicCamera(-fSize, fSize, fSize, -fSize, -fSize, fSize);

	pCameraControl = std::make_unique<TrackBallCameraControl>();
	pCameraControl->setCamera(pCamera);
}

void prepare()
{
	pNode = AssimpLoder::load("assets/fbx/Fist Fight B.fbx");

	pScene = Scene::Create();
	pRenderer = std::make_unique<Renderer>();
	//std::shared_ptr<SpotLight> pSpotLight = SpotLight::Create();

	//创建geometry
	std::shared_ptr<Geometry> pGeometry(Geometry::createBox(4.0f));
	//创建material
	std::shared_ptr<PhongMaterial> pMaterial = std::make_shared<PhongMaterial>();
	std::shared_ptr<Texture> pDiffuseTexture = Texture::createTexture("assets/textures/box.png", 0);
	std::shared_ptr<Texture> pSpecularMaskTexture = Texture::createTexture("assets/textures/sp_mask.png", 1);
	pMaterial->setShiness(16.0f);
	pMaterial->setDiffuseTexture(pDiffuseTexture);
	pMaterial->setSpecularMaskTexture(pSpecularMaskTexture);

	std::shared_ptr<Geometry> pWhiteLight(Geometry::createSphere(0.5f));
	std::shared_ptr<WhiteMaterial> pWhiteMaterial = std::make_shared<WhiteMaterial>();

	//生成mesh
	std::shared_ptr<Mesh> pMesh = Mesh::Create(pGeometry, pMaterial);
	std::shared_ptr<Mesh> pWhiteMesh = Mesh::Create(pWhiteLight, pWhiteMaterial);

	pWhiteMesh->setPosition(glm::vec3(3.0f, 0.5f, 0.5f));

	vctMesh.push_back(pMesh);
	vctMesh.push_back(pWhiteMesh);

	pPointLight = PointLight::Create();
	pPointLight->setPosition(glm::vec3(0.0f,0.0f,4.0f));
	pPointLight->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	pPointLight->setK2(0.017f);
	pPointLight->setK1(0.07f);
	pPointLight->setKc(1.0f);

	pSpotLight = SpotLight::Create();
	pSpotLight->setPosition(pWhiteMesh->getPosition());
	pSpotLight->rotateY(90.0f);
	pSpotLight->setInnerAngle(30.0f);
	pSpotLight->setOuterAngle(60.0f);

	pDirectionalLight = DirectionalLight::Create();
	pDirectionalLight->setPosition(glm::vec3(-3.0f, 0.0f, 0.0f));
	pDirectionalLight->rotateY(-90.0f);
	pDirectionalLight->setColor(glm::vec3(0.0f, 0.0f, 1.0f));

	pAmbientLight = std::make_shared<AmbientLight>();
	pAmbientLight->setColor(glm::vec3(0.2f));

	pMesh->setParent(pScene);
	pWhiteMesh->setParent(pMesh);
	pSpotLight->setParent(pMesh);
	pPointLight->setParent(pScene);
	pDirectionalLight->setParent(pScene);
	pScene->categorizedStorage();
	pScene->setAmbientLight(pAmbientLight);
}

void initIMGUI()
{
	ImGui::CreateContext();
	ImGui::StyleColorsLight();

	//设置ImGui与GLFW和Opengl的绑定
	ImGui_ImplGlfw_InitForOpenGL(appInstance->getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void prepareModel()
{
	//pNode = AssimpLoder::load("assets/fbx/Fist Fight B.fbx");
	pNode = AssimpLoder::load("assets/fbx/bag/backpack.obj");
	pNode->setScale(glm::vec3(0.5f, 0.5f, 0.5f));

	pScene = Scene::Create();
	pScene->setSelf(pScene);
	pScene->initEvent();
	pRenderer = std::make_unique<Renderer>();
	
	pPointLight = PointLight::Create();
	pPointLight->setPosition(glm::vec3(0.0f, 0.0f, 4.0f));
	pPointLight->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	pPointLight->setK2(0.017f);
	pPointLight->setK1(0.07f);
	pPointLight->setKc(1.0f);

	pSpotLight = SpotLight::Create();
	pSpotLight->setPosition(glm::vec3(3.0f, 0.5f, 0.5f));
	pSpotLight->rotateY(90.0f);
	pSpotLight->setInnerAngle(30.0f);
	pSpotLight->setOuterAngle(60.0f);

	pDirectionalLight = DirectionalLight::Create();
	pDirectionalLight->setPosition(glm::vec3(0.0f, 0.0f, 100.0f));
	pDirectionalLight->setColor(glm::vec3(1.0f, 1.0f, 1.0f));

	pAmbientLight = std::make_shared<AmbientLight>();
	pAmbientLight->setColor(glm::vec3(0.2f));

	//pSpotLight->setParent(pScene);
	//pPointLight->setParent(pScene);
	pDirectionalLight->setParent(pScene);
	pNode->setParent(pScene);
	pScene->categorizedStorage();
	pScene->setAmbientLight(pAmbientLight);
}

void preparemobanceshi()
{
	pScene = Scene::Create();
	pScene->setSelf(pScene);
	pScene->initEvent();
	pRenderer = std::make_unique<Renderer>();

	//1.普通方块
	std::shared_ptr<Geometry> pGeometryFirst(Geometry::createBox(2.0f));
	std::shared_ptr<PhongMaterial> pMaterialFirst = std::make_shared<PhongMaterial>();
	pMaterialFirst->setDiffuseTexture(Texture::createTexture("assets/textures/box.png", 0));
	std::shared_ptr<Texture> pSpecularMaskTexture = Texture::createTexture("assets/textures/sp_mask.png", 1);
	pMaterialFirst->setSpecularMaskTexture(pSpecularMaskTexture);

	std::shared_ptr<Mesh> pMesh = Mesh::Create(pGeometryFirst, pMaterialFirst);

	std::shared_ptr<WhiteMaterial> pWhiteMaterial = std::make_shared<WhiteMaterial>();
	std::shared_ptr<Mesh> pMeshBound = Mesh::Create(pGeometryFirst, pWhiteMaterial);
	pMeshBound->setPosition(pMesh->getPosition());
	pMeshBound->setScale(glm::vec3(1.3f));


	pDirectionalLight = DirectionalLight::Create();
	pDirectionalLight->setPosition(glm::vec3(0.0f, 0.0f, 100.0f));
	pDirectionalLight->setColor(glm::vec3(1.0f, 1.0f, 1.0f));

	pAmbientLight = std::make_shared<AmbientLight>();
	pAmbientLight->setColor(glm::vec3(0.2f));

	pMesh->setParent(pScene);
	pMeshBound->setParent(pScene);
	pDirectionalLight->setParent(pScene);
	pScene->setAmbientLight(pAmbientLight);
}

void renderIMGUI()
{
	//开启IMGUI渲染
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//增加控件
	ImGui::Begin("MyGui");
	ImGui::Text("ChangeColor Demo");
	ImGui::Button("Test Button", ImVec2(40, 20));
	ImGui::ColorEdit3("Clear Color", (float*)&clearColor);
	ImGui::End();

	//渲染
	ImGui::Render();
	int iDisplay_w = 0;
	int iDisplay_h = 0;
	glfwGetFramebufferSize(appInstance->getWindow(), &iDisplay_w, &iDisplay_h);
	glViewport(0, 0, iDisplay_w, iDisplay_h);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void setModelBlend(std::shared_ptr<ObjectBase> pNode, bool blend, float opacity) {
	if (auto pMesh = std::dynamic_pointer_cast<Mesh>(pNode))
	{
		auto pMaterial = pMesh->getMaterial();
		pMaterial->m_bBlend = blend;
		pMaterial->m_fOpacity = opacity;
		pMaterial->m_bDepthWrite = false;
	}

	auto children = pNode->getChildren();
	for (int i = 0; i < children.size(); i++) 
	{
		setModelBlend(children[i], blend, opacity);
	}
}

void prepareBlend() {
	pScene = Scene::Create();
	pScene->setSelf(pScene);
	pScene->initEvent();
	pRenderer = std::make_unique<Renderer>();
	auto pDefaultspmasktexture = Texture::createTexture("assets/textures/sp_mask.png", 1);

	//1 背包模型
	auto model = AssimpLoder::load("assets/fbx/bag/backpack.obj");
	setModelBlend(model, true, 0.2);
	model->setParent(pScene);

	//2 实体平面
	std::shared_ptr<Geometry> planeGeo(Geometry::createPlane(5.0, 5.0));
	auto planeMat = std::make_shared<PhongMaterial>();
	planeMat->setDiffuseTexture(Texture::createTexture("assets/textures/box.png", 0));
	planeMat->setSpecularMaskTexture(pDefaultspmasktexture);
	auto planeMesh =  Mesh::Create(planeGeo, planeMat);
	planeMesh->setPosition(glm::vec3(0.0f, 0.0f, 6.0f));
	planeMesh->setParent(pScene);

	//3 半透明平面
	std::shared_ptr<Geometry> planeGeoTrans(Geometry::createPlane(10.0, 10.0));
	auto planeMatTrans = std::make_shared<PhongMaterial>();
	planeMatTrans->setSpecularMaskTexture(pDefaultspmasktexture);
	planeMatTrans->setDiffuseTexture(Texture::createTexture("assets/textures/wall.jpg", 0));
	planeMatTrans->m_bBlend = true;
	planeMatTrans->m_fOpacity = 0.1;

	auto planeMeshTrans =  Mesh::Create(planeGeoTrans, planeMatTrans);
	planeMeshTrans->setPosition(glm::vec3(0.0f, 0.0f, -6.0f));

	planeMeshTrans->setParent(pScene);

	//4 实体平面
	std::shared_ptr<Geometry> planeGeo2(Geometry::createPlane(10.0, 10.0));
	auto planeMat2 = std::make_shared<PhongMaterial>();
	planeMat2->setDiffuseTexture(Texture::createTexture("assets/textures/goku.jpg", 0)); 
	planeMat2->setSpecularMaskTexture(pDefaultspmasktexture);

	auto planeMesh2 = Mesh::Create(planeGeo2, planeMat2);
	planeMesh2->setPosition(glm::vec3(3.0f, 0.0f, 0.0f));
	planeMesh2->rotateY(45.0f);
	planeMesh2->setParent(pScene);

	pDirectionalLight = DirectionalLight::Create();
	pDirectionalLight->setPosition(glm::vec3(0.0f, 0.0f, 100.0f));
	pDirectionalLight->setColor(glm::vec3(1.0f, 1.0f, 1.0f));

	pAmbientLight = std::make_shared<AmbientLight>();
	pAmbientLight->setColor(glm::vec3(0.2f));
	pDirectionalLight->setParent(pScene);

	pScene->setAmbientLight(pAmbientLight);
}

int main()
{
	if (!appInstance->Init(1600, 900))
	{
		return -1;
	}

	appInstance->setKeyCallback(keyEventCallback);
	appInstance->setResizeCallback(windowResizeCallback);
	appInstance->setMouseCallback(mouseButtonCallback);
	appInstance->setCursorCallback(cursorCallback);
	appInstance->setScrollCallback(scrollCallBack);
	
	GL_CALL(glViewport(0, 0, 1600, 900)); //设置视口大小 前2个参数是视口左下角位置，后2个参数是视口宽高
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f)); //设置清屏颜色

	initIMGUI();
	prepareCamera();
	prepareBlend();

	//执行窗体循环
	while (appInstance->Update())
	{
		pRenderer->setClearColor(clearColor);
		//vctMesh[0]->rotateY(0.005f);
		pCameraControl->update();

		//pRenderer->render(vctMesh, pCamera, pDirectionalLight, pAmbientLight);
		//pRenderer->render(vctMesh, pCamera, pPointLight, pAmbientLight);
		//pRenderer->render(vctMesh, pCamera, pSpotLight, pAmbientLight);
		pRenderer->render(pScene, pCamera);
		renderIMGUI();
	}

	// 5. 释放相机和控制器
	pCameraControl.reset();
	pCamera.reset(); // 如果pCamera已经从pCameraControl转移

	pDirectionalLight.reset();
	pPointLight.reset();
	pSpotLight.reset();
	pAmbientLight.reset();
	pRenderer.reset();
	pNode.reset();
	pScene.reset();
	for (int i = 0; i < vctMesh.size(); i++)
	{
		vctMesh[i].reset();
	}

	appInstance->Destroy();
	
	return 0;
}