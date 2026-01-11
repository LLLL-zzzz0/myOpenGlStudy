#include "whiteMaterial.h"

WhiteMaterial::WhiteMaterial()
{
	m_enumType = MaterialType::WhiteMaterial;
}

WhiteMaterial::~WhiteMaterial()
{
}

void WhiteMaterial::bind(Shader* pShader, Camera* pCamera, glm::mat3 normalMatrix, const std::vector<SpotLight*>& vctSpotLight, const std::vector<DirectionalLight*>& vctDirectionalLight, const std::vector<PointLight*>& vctPointLight, AmbientLight* pAmbientLight)
{

}
