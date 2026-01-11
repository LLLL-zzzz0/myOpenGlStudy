#include "depthMaterial.h"

DepthMaterial::DepthMaterial() 
{
	m_enumType = MaterialType::DepthMaterial;
}

DepthMaterial::~DepthMaterial() 
{

}

void DepthMaterial::bind(
	Shader* pShader,
	Camera* pCamera,
	glm::mat3 normalMatrix,
	const std::vector<SpotLight*>& vctSpotLight,
	const std::vector<DirectionalLight*>& vctDirectionalLight,
	const std::vector<PointLight*>& vctPointLight,
	AmbientLight* pAmbientLight
)
{
	pShader->setFloat("near", pCamera->getNear());
	pShader->setFloat("far", pCamera->getFar());
}
