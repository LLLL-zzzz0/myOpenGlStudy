#pragma once
#include "material.h"

class DepthMaterial :public Material {
public:
	DepthMaterial();
	~DepthMaterial();
	void bind(
		Shader* pShader,
		Camera* pCamera,
		glm::mat3 normalMatrix,
		const std::vector<SpotLight*>& vctSpotLight,
		const std::vector<DirectionalLight*>& vctDirectionalLight,
		const std::vector<PointLight*>& vctPointLight,
		AmbientLight* pAmbientLight
	)override;
};