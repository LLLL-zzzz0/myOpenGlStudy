#pragma once

#include "material.h"
#include "../texture.h"
#include <memory>

class PhongMaterial :public Material
{
public:
	PhongMaterial();
	~PhongMaterial();
	void bind(Shader* pShader,
		Camera* pCamera,
		glm::mat3 normalMatrix,
		const std::vector<SpotLight*>& vctSpotLight,
		const std::vector<DirectionalLight*>& vctDirectionalLight,
		const std::vector<PointLight*>& vctPointLight,
		AmbientLight* pAmbientLight) override;
	void setShiness(const float& fShiness) { m_fShiness = fShiness; }
	void setDiffuseTexture(std::shared_ptr<Texture> pTexture) { m_pDiffuseTexture = pTexture; }
	void setSpecularMaskTexture(std::shared_ptr<Texture> pTexture) { m_pSpecularMaskTexture = pTexture;}
	float getShiness() { return m_fShiness; }
	Texture* getDiffuseTexture() { return m_pDiffuseTexture.get(); }
	Texture* getSpecularMaskTexture() { return m_pSpecularMaskTexture.get(); }
private:
	std::shared_ptr<Texture> m_pDiffuseTexture;
	std::shared_ptr<Texture> m_pSpecularMaskTexture;
	float m_fShiness{ 1.0f };
};