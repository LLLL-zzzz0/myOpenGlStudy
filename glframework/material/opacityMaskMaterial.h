#pragma once
#include "material.h"
#include "../texture.h"

class OpacityMaskMaterial :public Material 
{
public:
	OpacityMaskMaterial();
	~OpacityMaskMaterial();
	void setShiness(const float& fShiness) { m_fShiness = fShiness; }
	void setDiffuseTexture(std::shared_ptr<Texture> pTexture) { m_pDiffuseTexture = pTexture; }
	void setOpacityMaskTexture(std::shared_ptr<Texture> pTexture) { m_pOpacityMaskTexture = pTexture; }
	float getShiness() { return m_fShiness; }
	Texture* getDiffuseTexture() { return m_pDiffuseTexture.get(); }
	Texture* getOpacityMaskTexture() { return m_pOpacityMaskTexture.get(); }

	void bind(Shader* pShader,
		Camera* pCamera,
		glm::mat3 normalMatrix,
		const std::vector<SpotLight*>& vctSpotLight,
		const std::vector<DirectionalLight*>& vctDirectionalLight,
		const std::vector<PointLight*>& vctPointLight,
		AmbientLight* pAmbientLight) override;

private:
	std::shared_ptr<Texture> m_pDiffuseTexture;
	std::shared_ptr<Texture> m_pOpacityMaskTexture;
	float m_fShiness{ 1.0f };
};