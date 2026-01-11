#include "renderer.h"
#include <iostream>
#include "../material/phongMaterial.h"
#include "../material/whiteMaterial.h"


Renderer::Renderer()
{
	m_mapShaderFile[MaterialType::PhongMaterial] = "assets/shaders/phong";
	m_mapShaderFile[MaterialType::WhiteMaterial] = "assets/shaders/white";
	m_mapShaderFile[MaterialType::DepthMaterial] = "assets/shaders/depth";
}

Renderer::~Renderer()
{
}

void Renderer::render(Scene* pScene, Camera* pCamera)
{
	pScene->categorizedStorageIfNeeded(pCamera);

	//先取出mesh以及lights
	std::vector<SpotLight*> vctSpotLights = pScene->getSpotLights();
	std::vector<PointLight*> vctPointLights = pScene->getPointLights();
	std::vector<DirectionalLight*> vctDirLights = pScene->getDirLights();
	std::vector<Mesh*> vctOpaqueMeshs = pScene->getOpaqueMeshs();
	std::vector<Mesh*> vctTransparentMeshs = pScene->getTransparentMeshs();

	//状态机参数
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glDepthFunc(GL_LESS));
	GL_CALL(glDepthMask(GL_TRUE));

	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);

	//颜色混合
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//清理画布
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
	//绘制不透明mesh
	for (int i = 0; i < vctOpaqueMeshs.size(); i++)
	{
		renderMesh(vctOpaqueMeshs[i], pCamera, vctSpotLights, vctDirLights, vctPointLights, pScene->getAmbientLight());
	}

	//绘制透明mesh
	for (int i = 0; i < vctTransparentMeshs.size(); i++)
	{
		renderMesh(vctTransparentMeshs[i], pCamera, vctSpotLights, vctDirLights, vctPointLights, pScene->getAmbientLight());
	}
}

void Renderer::renderMesh(
	Mesh* pMesh,
	Camera* pCamera,
	const std::vector<SpotLight*>& vctSpotLight,
	const std::vector<DirectionalLight*>& vctDirectionalLight,
	const std::vector<PointLight*>& vctPointLight,
	AmbientLight* pAmbientLight
)
{
	Geometry* pGeometry = pMesh->getGeometry();
	Material* pMaterial = pMesh->getMaterial();

	pMaterial->setOpenGLStatus();

	Shader* pShader = selectShader(pMaterial->getMaterialType());
	if (nullptr == pShader)
	{
		std::cout << "Unknow MaterialType" << std::endl;
		return;
	}

	pShader->beginShader();
	pMaterial->bind(pShader, pCamera, pMesh->getNormalMatrix(), vctSpotLight, vctDirectionalLight, vctPointLight, pAmbientLight);

	//mvp变换矩阵
	pShader->setMatrix4x4("modelMatrix", pMesh->getWorldMatrix());
	pShader->setMatrix4x4("viewMarTrix", pCamera->getViewMatrix());
	pShader->setMatrix4x4("projectionMatrix", pCamera->getProjectionMatrix());

	//绑定VAO
	GL_CALL(glBindVertexArray(pGeometry->getVao()));

	//绘制  无偏移量
	GL_CALL(glDrawElements(GL_TRIANGLES, pGeometry->getIndicesCount(), GL_UNSIGNED_INT, 0));

	//解绑VAO
	GL_CALL(glBindVertexArray(0));

	pShader->endShader();
}


void Renderer::setClearColor(glm::vec3 vec3Color)
{
	GL_CALL(glClearColor(vec3Color.r, vec3Color.g, vec3Color.b, 1.0f));
}

Shader* Renderer::selectShader(MaterialType materialType)
{
	Shader* pShader = nullptr;
	if (m_mapShaderFile.count(materialType))
	{
		if (!m_mapShaderCache.count(materialType))
		{
			std::string strVert = m_mapShaderFile[materialType] + ".vert";
			std::string strFrag = m_mapShaderFile[materialType] + ".frag";
			m_mapShaderCache[materialType] = std::make_unique<Shader>(strVert.c_str(), strFrag.c_str());
		}
		pShader = m_mapShaderCache[materialType].get();
	}
	else
	{
		std::cerr << "Unknow MaterialType: " << static_cast<int>(materialType) << std::endl;
	}

	return pShader;
}
