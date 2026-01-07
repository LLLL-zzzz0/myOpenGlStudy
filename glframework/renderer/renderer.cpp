#include "renderer.h"
#include <iostream>
#include "../material/phongMaterial.h"
#include "../material/whiteMaterial.h"


Renderer::Renderer()
{
	m_pPhongShader = std::make_unique<Shader>("assets/shaders/phong.vert", "assets/shaders/phong.frag");
	m_pWhiteShader = std::make_unique<Shader>("assets/shaders/white.vert", "assets/shaders/white.frag");
}

Renderer::~Renderer()
{
}

void Renderer::render(const std::vector<std::shared_ptr<Mesh>>& vctMesh, std::shared_ptr<Camera> pCamera, std::shared_ptr<DirectionalLight> pDirectionalLight, std::shared_ptr<AmbientLight> pAmbientLight)
{
	//设置opengl的状态参数
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glDepthFunc(GL_LESS));
	//清理画布
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	//绘制mesh

	for (int i = 0; i < vctMesh.size(); i++)
	{
		std::shared_ptr<Geometry> pGeometry = vctMesh[i]->getGeometry();
		std::shared_ptr<Material> pMaterial = vctMesh[i]->getMaterial();

		Shader* pShader = selectShader(pMaterial->getMaterialType());
		if (nullptr == pShader)
		{
			std::cout << "Unknow MaterialType" << std::endl;
			continue;
		}

		pShader->beginShader();
		switch (pMaterial->getMaterialType())
		{
		    case MaterialType::PhongMaterial:
		    {
				PhongMaterial* pPhongMaterial = dynamic_cast<PhongMaterial*>(pMaterial.get());

				pShader->setInt("sampler", 0);
				pPhongMaterial->getDiffuseTexture()->bind();

				pShader->setInt("specularMaskSampler", 1);
				pPhongMaterial->getSpecularMaskTexture()->bind();

				pShader->setVec3("lightDirection", pDirectionalLight->getWorldDirection());
				pShader->setVec3("lightColor", pDirectionalLight->getColor());
				pShader->setVec3("ambientColor", pAmbientLight->getColor());
				pShader->setFloat("specularIntensity", pDirectionalLight->getSpecularIntensity());
				pShader->setFloat("shiness", pPhongMaterial->getShiness());

				pShader->setMatrix4x4("modelMatrix", vctMesh[i]->getWorldMatrix());
				pShader->setMatrix4x4("viewMarTrix", pCamera->getViewMatrix());
				pShader->setMatrix4x4("projectionMatrix", pCamera->getProjectionMatrix());

				// 设置相机位置（用于光照计算）
				pShader->setVec3("cameraPosition", pCamera->getPosition());

				//计算NormalMatrix
				glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(vctMesh[i]->getWorldMatrix())));
				pShader->setMatrix3x3("normalMatrix", normalMatrix);
		    	break;
		    }
			case MaterialType::WhiteMaterial:
			{
				pShader->setMatrix4x4("modelMatrix", vctMesh[i]->getWorldMatrix());
				pShader->setMatrix4x4("viewMarTrix", pCamera->getViewMatrix());
				pShader->setMatrix4x4("projectionMatrix", pCamera->getProjectionMatrix());
				break;
			}
		}

		//绑定VAO
		GL_CALL(glBindVertexArray(pGeometry->getVao()));

		//绘制  无偏移量
		GL_CALL(glDrawElements(GL_TRIANGLES, pGeometry->getIndicesCount(), GL_UNSIGNED_INT, 0));

		//解绑VAO
		GL_CALL(glBindVertexArray(0));

		pShader->endShader();
	}
}

void Renderer::render(const std::vector<std::shared_ptr<Mesh>>& vctMesh, std::shared_ptr<Camera> pCamera, std::shared_ptr<PointLight> pPointLight, std::shared_ptr<AmbientLight> pAmbientLight)
{
	//设置opengl的状态参数
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glDepthFunc(GL_LESS));
	//清理画布
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	//绘制mesh

	for (int i = 0; i < vctMesh.size(); i++)
	{
		std::shared_ptr<Geometry> pGeometry = vctMesh[i]->getGeometry();
		std::shared_ptr<Material> pMaterial = vctMesh[i]->getMaterial();

		Shader* pShader = selectShader(pMaterial->getMaterialType());
		if (nullptr == pShader)
		{
			std::cout << "Unknow MaterialType" << std::endl;
			continue;
		}

		pShader->beginShader();
		switch (pMaterial->getMaterialType())
		{
		case MaterialType::PhongMaterial:
		{
			PhongMaterial* pPhongMaterial = dynamic_cast<PhongMaterial*>(pMaterial.get());

			pShader->setInt("sampler", 0);
			pPhongMaterial->getDiffuseTexture()->bind();

			pShader->setInt("specularMaskSampler", 1);
			pPhongMaterial->getSpecularMaskTexture()->bind();

			pShader->setVec3("lightPosition", pPointLight->getPosition());
			pShader->setVec3("lightColor", pPointLight->getColor());
			pShader->setVec3("ambientColor", pAmbientLight->getColor());
			pShader->setFloat("specularIntensity", pPointLight->getSpecularIntensity());
			pShader->setFloat("shiness", pPhongMaterial->getShiness());

			pShader->setFloat("k1", pPointLight->getK1());
			pShader->setFloat("k2", pPointLight->getK2());
			pShader->setFloat("kc", pPointLight->getKc());

			pShader->setMatrix4x4("modelMatrix", vctMesh[i]->getWorldMatrix());
			pShader->setMatrix4x4("viewMarTrix", pCamera->getViewMatrix());
			pShader->setMatrix4x4("projectionMatrix", pCamera->getProjectionMatrix());

			// 设置相机位置（用于光照计算）
			pShader->setVec3("cameraPosition", pCamera->getPosition());

			//计算NormalMatrix
			glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(vctMesh[i]->getWorldMatrix())));
			pShader->setMatrix3x3("normalMatrix", normalMatrix);
			break;
		}
		case MaterialType::WhiteMaterial:
		{
			pShader->setMatrix4x4("modelMatrix", vctMesh[i]->getWorldMatrix());
			pShader->setMatrix4x4("viewMarTrix", pCamera->getViewMatrix());
			pShader->setMatrix4x4("projectionMatrix", pCamera->getProjectionMatrix());
			break;
		}
		}

		//绑定VAO
		GL_CALL(glBindVertexArray(pGeometry->getVao()));

		//绘制  无偏移量
		GL_CALL(glDrawElements(GL_TRIANGLES, pGeometry->getIndicesCount(), GL_UNSIGNED_INT, 0));

		//解绑VAO
		GL_CALL(glBindVertexArray(0));

		pShader->endShader();
	}
}


void Renderer::render(const std::vector<std::shared_ptr<Mesh>>& vctMesh, std::shared_ptr<Camera> pCamera, std::shared_ptr<SpotLight> pSpotLight, std::shared_ptr<AmbientLight> pAmbientLight)
{
	//设置opengl的状态参数
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glDepthFunc(GL_LESS));
	//清理画布
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	//绘制mesh

	for (int i = 0; i < vctMesh.size(); i++)
	{
		std::shared_ptr<Geometry> pGeometry = vctMesh[i]->getGeometry();
		std::shared_ptr<Material> pMaterial = vctMesh[i]->getMaterial();

		Shader* pShader = selectShader(pMaterial->getMaterialType());
		if (nullptr == pShader)
		{
			std::cout << "Unknow MaterialType" << std::endl;
			continue;
		}

		pShader->beginShader();
		switch (pMaterial->getMaterialType())
		{
		case MaterialType::PhongMaterial:
		{
			PhongMaterial* pPhongMaterial = dynamic_cast<PhongMaterial*>(pMaterial.get());

			pShader->setInt("sampler", 0);
			pPhongMaterial->getDiffuseTexture()->bind();

			pShader->setInt("specularMaskSampler", 1);
			pPhongMaterial->getSpecularMaskTexture()->bind();

			pShader->setVec3("spotLight.position", pSpotLight->getWorldPosition());
			pShader->setVec3("spotLight.targetDirection", pSpotLight->getWorldDirection());
			pShader->setVec3("spotLight.color", pSpotLight->getColor());
			pShader->setFloat("spotLight.specularIntensity", pSpotLight->getSpecularIntensity());
			pShader->setFloat("spotLight.innerLine", glm::cos(glm::radians(pSpotLight->getInnerAngle())));
			pShader->setFloat("spotLight.outerLine", glm::cos(glm::radians(pSpotLight->getOuterAngle())));

			pShader->setFloat("shiness", pPhongMaterial->getShiness());

			pShader->setVec3("ambientColor", pAmbientLight->getColor());

			pShader->setMatrix4x4("modelMatrix", vctMesh[i]->getWorldMatrix());
			pShader->setMatrix4x4("viewMarTrix", pCamera->getViewMatrix());
			pShader->setMatrix4x4("projectionMatrix", pCamera->getProjectionMatrix());

			// 设置相机位置（用于光照计算）
			pShader->setVec3("cameraPosition", pCamera->getPosition());

			//计算NormalMatrix
			glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(vctMesh[i]->getWorldMatrix())));
			pShader->setMatrix3x3("normalMatrix", normalMatrix);
			break;
		}
		case MaterialType::WhiteMaterial:
		{
			pShader->setMatrix4x4("modelMatrix", vctMesh[i]->getWorldMatrix());
			pShader->setMatrix4x4("viewMarTrix", pCamera->getViewMatrix());
			pShader->setMatrix4x4("projectionMatrix", pCamera->getProjectionMatrix());
			break;
		}
		}

		//绑定VAO
		GL_CALL(glBindVertexArray(pGeometry->getVao()));

		//绘制  无偏移量
		GL_CALL(glDrawElements(GL_TRIANGLES, pGeometry->getIndicesCount(), GL_UNSIGNED_INT, 0));

		//解绑VAO
		GL_CALL(glBindVertexArray(0));

		pShader->endShader();
	}
}

void Renderer::render(std::shared_ptr<Scene> pScene, std::shared_ptr<Camera> pCamera)
{
	using PtrSpotLight = std::shared_ptr<SpotLight>;
	using PtrPointLight = std::shared_ptr<PointLight>;
	using PtrDirLight = std::shared_ptr<DirectionalLight>;
	using PtrMesh = std::shared_ptr<Mesh>;

	//先取出mesh以及lights
	std::vector<PtrSpotLight> vctSpotLights = pScene->getSpotLights();
	std::vector<PtrPointLight> vctPointLights = pScene->getPointLights();
	std::vector<PtrDirLight> vctDirLights = pScene->getDirLights();
	std::vector<PtrMesh> vctMeshs = pScene->getMeshs();

	//设置opengl的状态参数
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glDepthFunc(GL_LESS));
	//清理画布
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	//绘制mesh
	for (int i = 0; i < vctMeshs.size(); i++)
	{
		std::shared_ptr<Geometry> pGeometry = vctMeshs[i]->getGeometry();
		std::shared_ptr<Material> pMaterial = vctMeshs[i]->getMaterial();

		Shader* pShader = selectShader(pMaterial->getMaterialType());
		if (nullptr == pShader)
		{
			std::cout << "Unknow MaterialType" << std::endl;
			continue;
		}

		pShader->beginShader();
		switch (pMaterial->getMaterialType())
		{
		case MaterialType::PhongMaterial:
		{
			PhongMaterial* pPhongMaterial = dynamic_cast<PhongMaterial*>(pMaterial.get());
			pShader->setInt("sampler", 0);
			pPhongMaterial->getDiffuseTexture()->bind();

			pShader->setInt("specularMaskSampler", 1);
			pPhongMaterial->getSpecularMaskTexture()->bind();

			int iMin = 0;
			iMin = glm::min((int)(vctSpotLights.size()), MAX_LIGHT_NUM);
			pShader->setInt("spotLightNum", iMin);
			for (int i = 0; i < iMin; i++)
			{
				std::string strHead = "spotLights[" + std::to_string(i);
				pShader->setVec3(strHead + "].position", vctSpotLights[i]->getWorldPosition());
				pShader->setVec3(strHead + "].targetDirection", vctSpotLights[i]->getWorldDirection());
				pShader->setVec3(strHead + "].color", vctSpotLights[i]->getColor());
				pShader->setFloat(strHead + "].specularIntensity", vctSpotLights[i]->getSpecularIntensity());
				pShader->setFloat(strHead + "].innerLine", glm::cos(glm::radians(vctSpotLights[i]->getInnerAngle())));
				pShader->setFloat(strHead + "].outerLine", glm::cos(glm::radians(vctSpotLights[i]->getOuterAngle())));
			}

			iMin = 0;
			iMin = glm::min((int)(vctPointLights.size()), MAX_LIGHT_NUM);
			pShader->setInt("pointLightNum", iMin);
			for (int i = 0; i < iMin; i++)
			{
				std::string strHead = "pointLights[" + std::to_string(i);
				pShader->setVec3(strHead + "].position", vctPointLights[i]->getWorldPosition());
				pShader->setVec3(strHead + "].color", vctPointLights[i]->getColor());
				pShader->setFloat(strHead + "].specularIntensity", vctPointLights[i]->getSpecularIntensity());
				pShader->setFloat(strHead + "].k1", vctPointLights[i]->getK1());
				pShader->setFloat(strHead + "].k2", vctPointLights[i]->getK2());
				pShader->setFloat(strHead + "].kc", vctPointLights[i]->getKc());
			}

			iMin = 0;
			iMin = glm::min((int)(vctDirLights.size()), MAX_LIGHT_NUM);
			pShader->setInt("dirLightNum", iMin);
			for (int i = 0; i < iMin; i++)
			{
				std::string strHead = "directionalLights[" + std::to_string(i);
				pShader->setVec3(strHead + "].direction", vctDirLights[i]->getWorldDirection());
				pShader->setVec3(strHead + "].color", vctDirLights[i]->getColor());
				pShader->setFloat(strHead + "].specularIntensity", vctDirLights[i]->getSpecularIntensity());
			}

			pShader->setFloat("shiness", pPhongMaterial->getShiness());

			pShader->setVec3("ambientColor", pScene->getAmbientLight()->getColor());

			pShader->setMatrix4x4("modelMatrix", vctMeshs[i]->getWorldMatrix());
			pShader->setMatrix4x4("viewMarTrix", pCamera->getViewMatrix());
			pShader->setMatrix4x4("projectionMatrix", pCamera->getProjectionMatrix());

			// 设置相机位置（用于光照计算）
			pShader->setVec3("cameraPosition", pCamera->getPosition());

			//计算NormalMatrix
			glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(vctMeshs[i]->getWorldMatrix())));
			pShader->setMatrix3x3("normalMatrix", normalMatrix);
			
			break;
		}
		case MaterialType::WhiteMaterial:
		{
			pShader->setMatrix4x4("modelMatrix", vctMeshs[i]->getWorldMatrix());
			pShader->setMatrix4x4("viewMarTrix", pCamera->getViewMatrix());
			pShader->setMatrix4x4("projectionMatrix", pCamera->getProjectionMatrix());
			break;
		}
		}

		//绑定VAO
		GL_CALL(glBindVertexArray(pGeometry->getVao()));

		//绘制  无偏移量
		GL_CALL(glDrawElements(GL_TRIANGLES, pGeometry->getIndicesCount(), GL_UNSIGNED_INT, 0));

		//解绑VAO
		GL_CALL(glBindVertexArray(0));

		pShader->endShader();
	}

}

Shader* Renderer::selectShader(MaterialType materialType)
{
	Shader* pShader = nullptr;
	switch (materialType)
	{
	    case MaterialType::PhongMaterial:
	    {
	    	pShader = m_pPhongShader.get();
	    	break;
	    }
		case MaterialType::WhiteMaterial:
		{
			pShader = m_pWhiteShader.get();
			break;
		}
	    default:
	    {
	    	break;
	    }
	}

	return pShader;
}
