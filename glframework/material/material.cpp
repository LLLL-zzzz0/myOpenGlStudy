#include "material.h"
#include "material.h"

Material::Material()
{
}

Material::~Material()
{
}

MaterialType Material::getMaterialType()
{
	return m_enumType;
}

void Material::setOpenGLStatus()
{
	setDepthState();
	setPolygomOffsetState();
	setStencilState();
	setBlendState();
	setFaceCulling();
}

void Material::setShaderSpotLight(Shader* pShader, const std::vector<SpotLight*>& vctSpotLight)
{
	int iMin = 0;
	iMin = glm::min((int)(vctSpotLight.size()), MAX_LIGHT_NUM);
	pShader->setInt("spotLightNum", iMin);
	for (int i = 0; i < iMin; i++)
	{
		std::string strHead = "spotLights[" + std::to_string(i);
		pShader->setVec3(strHead + "].position", vctSpotLight[i]->getWorldPosition());
		pShader->setVec3(strHead + "].targetDirection", vctSpotLight[i]->getWorldDirection());
		pShader->setVec3(strHead + "].color", vctSpotLight[i]->getColor());
		pShader->setFloat(strHead + "].specularIntensity", vctSpotLight[i]->getSpecularIntensity());
		pShader->setFloat(strHead + "].innerLine", glm::cos(glm::radians(vctSpotLight[i]->getInnerAngle())));
		pShader->setFloat(strHead + "].outerLine", glm::cos(glm::radians(vctSpotLight[i]->getOuterAngle())));
	}
}

void Material::setShaderDirLight(Shader* pShader, const std::vector<DirectionalLight*>& vctDirectionalLight)
{
	int iMin = 0;
	iMin = glm::min((int)(vctDirectionalLight.size()), MAX_LIGHT_NUM);
	pShader->setInt("dirLightNum", iMin);
	for (int i = 0; i < iMin; i++)
	{
		std::string strHead = "directionalLights[" + std::to_string(i);
		pShader->setVec3(strHead + "].direction", vctDirectionalLight[i]->getWorldDirection());
		pShader->setVec3(strHead + "].color", vctDirectionalLight[i]->getColor());
		pShader->setFloat(strHead + "].specularIntensity", vctDirectionalLight[i]->getSpecularIntensity());
	}
}

void Material::setShaderPointLight(Shader* pShader, const std::vector<PointLight*>& vctPointLight)
{
	int iMin = 0;
	iMin = glm::min((int)(vctPointLight.size()), MAX_LIGHT_NUM);
	pShader->setInt("pointLightNum", iMin);
	for (int i = 0; i < iMin; i++)
	{
		std::string strHead = "pointLights[" + std::to_string(i);
		pShader->setVec3(strHead + "].position", vctPointLight[i]->getWorldPosition());
		pShader->setVec3(strHead + "].color", vctPointLight[i]->getColor());
		pShader->setFloat(strHead + "].specularIntensity", vctPointLight[i]->getSpecularIntensity());
		pShader->setFloat(strHead + "].k1", vctPointLight[i]->getK1());
		pShader->setFloat(strHead + "].k2", vctPointLight[i]->getK2());
		pShader->setFloat(strHead + "].kc", vctPointLight[i]->getKc());
	}
}

void Material::setDepthState()
{
	//…Ë÷√…Ó∂»ºÏ≤‚œ‡πÿ‰÷»æ◊¥Ã¨
	if (m_bDepthTest)
	{
		GL_CALL(glEnable(GL_DEPTH_TEST));
		GL_CALL(glDepthFunc(m_enumDepthFunc));
	}
	else
	{
		GL_CALL(glDisable(GL_DEPTH_TEST));
	}

	if (m_bDepthWrite)
	{
		GL_CALL(glDepthMask(GL_TRUE));
	}
	else
	{
		GL_CALL(glDepthMask(GL_FALSE));
	}
}

void Material::setPolygomOffsetState()
{
	//ºÏ≤‚polygonOffset
	if (m_bPolygonOffset)
	{
		GL_CALL(glEnable(m_uiPolygonOffsetType));
		GL_CALL(glPolygonOffset(m_fDepthFactor, m_fDepthUnit));
	}
	else
	{
		GL_CALL(glDisable(GL_POLYGON_OFFSET_FILL));
		GL_CALL(glDisable(GL_POLYGON_OFFSET_LINE));
	}
}

void Material::setStencilState()
{
	if (m_bStencilTest)
	{
		GL_CALL(glEnable(GL_STENCIL_TEST));

		GL_CALL(glStencilOp(m_uiSFail, m_uiZFail, m_uiZPass));
		GL_CALL(glStencilMask(m_uiStencilMask));
		GL_CALL(glStencilFunc(m_uiStencilFunc, m_uiStencilRef, m_uiStencilFuncMask));
	}
	else
	{
		GL_CALL(glDisable(GL_STENCIL_TEST));
	}
}

void Material::setBlendState()
{
	if (m_bBlend)
	{
		GL_CALL(glEnable(GL_BLEND));
		GL_CALL(glBlendFunc(m_uiSBlendFactor, m_uiDBlendFactor));
	}
	else
	{
		GL_CALL(glDisable(GL_BLEND));
	}
}

void Material::setFaceCulling()
{
	if (m_bFaceCulling)
	{
		GL_CALL(glEnable(GL_CULL_FACE));
		GL_CALL(glFrontFace(m_uiFrontFace));
		GL_CALL(glCullFace(m_uiCullFace));
	}
	else
	{
		GL_CALL(glDisable(GL_CULL_FACE));
	}
}
