#pragma once

#include "../core.h"
#include <vector>
#include "../../Application/camera/camera.h"
#include "../light/directionalLight.h"
#include "../light/ambientLight.h"
#include "../light/pointLight.h"
#include "../light/spotLight.h"
#include "../mesh.h"
#include "../../wrapper/checkError.h"
#include "../shader.h"
#include "../scene.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	//渲染功能函数:
	//1.每次调用都会渲染一帧

	void renderMesh(
		Mesh* pMesh,
		Camera* pCamera,
		const std::vector<SpotLight*>& vctSpotLight,
		const std::vector<DirectionalLight*>& vctDirectionalLight,
		const std::vector<PointLight*>& vctPointLight,
		AmbientLight* pAmbientLight
	);

	void render(Scene* pScene, Camera* pCamera);

	void setClearColor(glm::vec3 vec3Color);

private:
	Shader* selectShader(MaterialType materialType);

	std::unordered_map<MaterialType, std::unique_ptr<Shader>> m_mapShaderCache{};
	std::map<MaterialType, std::string> m_mapShaderFile{};
};