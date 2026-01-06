#pragma once

#include "material.h"
#include "../texture.h"
#include <memory>

class PhongMaterial :public Material
{
public:
	PhongMaterial();
	~PhongMaterial();
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