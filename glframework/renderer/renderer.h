#pragma once

#define MAX_LIGHT_NUM 5

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
	void render(
		const std::vector<std::shared_ptr<Mesh>> & vctMesh,
		std::shared_ptr<Camera> pCamera,
		std::shared_ptr<DirectionalLight> pDirectionalLight,
		std::shared_ptr<AmbientLight> pAmbientLight
		);

	void render(
		const std::vector<std::shared_ptr<Mesh>>& vctMesh,
		std::shared_ptr<Camera> pCamera,
		std::shared_ptr<PointLight> pPointLight,
		std::shared_ptr<AmbientLight> pAmbientLight
	);

	void renderMesh(
		std::shared_ptr<Mesh> pMesh,
		std::shared_ptr<Camera> pCamera,
		const std::vector<std::shared_ptr<SpotLight>>& vctSpotLight,
		const std::vector<std::shared_ptr<DirectionalLight>>& vctDirectionalLight,
		const std::vector<std::shared_ptr<PointLight>>& vctPointLight,
		std::shared_ptr<AmbientLight> pAmbientLight
	);

	void render(
		const std::vector<std::shared_ptr<Mesh>>& vctMesh,
		std::shared_ptr<Camera> pCamera,
		std::shared_ptr<SpotLight> pSpotLight,
		std::shared_ptr<AmbientLight> pAmbientLight
	);

	void render(std::shared_ptr<Scene> pScene, std::shared_ptr<Camera> pCamera);

	void setClearColor(glm::vec3 vec3Color);

	
private:
	Shader* selectShader(MaterialType materialType);
	void setDepthState(std::shared_ptr<Material> pMaterial);
	void setPolygomOffsetState(std::shared_ptr<Material> pMaterial);
	void setStencilState(std::shared_ptr<Material> pMaterial);
	void setBlendState(std::shared_ptr<Material> pMaterial);

	std::unique_ptr<Shader> m_pPhongShader;
	std::unique_ptr<Shader> m_pWhiteShader;
};