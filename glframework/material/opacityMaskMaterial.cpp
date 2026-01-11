#include "opacityMaskMaterial.h"

OpacityMaskMaterial::OpacityMaskMaterial() 
{
	m_enumType = MaterialType::OpacityMaskMaterial;
}

OpacityMaskMaterial::~OpacityMaskMaterial() 
{

}

void OpacityMaskMaterial::bind(
	Shader* pShader,
	Camera* pCamera,
	glm::mat3 normalMatrix,
	const std::vector<SpotLight*>& vctSpotLight,
	const std::vector<DirectionalLight*>& vctDirectionalLight,
	const std::vector<PointLight*>& vctPointLight,
	AmbientLight* pAmbientLight
)
{
	pShader->setInt("sampler", 0);
	m_pDiffuseTexture->bind();

	
	pShader->setInt("specularMaskSampler", 1);
	m_pOpacityMaskTexture->bind();


	setShaderDirLight(pShader, vctDirectionalLight);
	setShaderSpotLight(pShader, vctSpotLight);
	setShaderPointLight(pShader, vctPointLight);

	pShader->setFloat("shiness", m_fShiness);

	pShader->setVec3("ambientColor", pAmbientLight->getColor());

	// 设置相机位置（用于光照计算）
	pShader->setVec3("cameraPosition", pCamera->getPosition());

	//计算NormalMatrix
	pShader->setMatrix3x3("normalMatrix", normalMatrix);

	pShader->setFloat("opacity", m_fOpacity);
}