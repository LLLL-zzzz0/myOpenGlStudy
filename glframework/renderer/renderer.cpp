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
		Geometry* pGeometry = vctMesh[i]->getGeometry();
		Material* pMaterial = vctMesh[i]->getMaterial();

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
				PhongMaterial* pPhongMaterial = dynamic_cast<PhongMaterial*>(pMaterial);

				pShader->setInt("sampler", 0);
				pPhongMaterial->getDiffuseTexture()->bind();

				pShader->setInt("specularMaskSampler", 1);
				pPhongMaterial->getSpecularMaskTexture()->bind();

				pShader->setVec3("lightDirection", pDirectionalLight->getDirection());
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
		Geometry* pGeometry = vctMesh[i]->getGeometry();
		Material* pMaterial = vctMesh[i]->getMaterial();

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
			PhongMaterial* pPhongMaterial = dynamic_cast<PhongMaterial*>(pMaterial);

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
		Geometry* pGeometry = vctMesh[i]->getGeometry();
		Material* pMaterial = vctMesh[i]->getMaterial();

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
			PhongMaterial* pPhongMaterial = dynamic_cast<PhongMaterial*>(pMaterial);

			pShader->setInt("sampler", 0);
			pPhongMaterial->getDiffuseTexture()->bind();

			pShader->setInt("specularMaskSampler", 1);
			pPhongMaterial->getSpecularMaskTexture()->bind();

			pShader->setVec3("spotLight.position", pSpotLight->getPosition());
			pShader->setVec3("spotLight.targetDirection", pSpotLight->getTargetDirection());
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
