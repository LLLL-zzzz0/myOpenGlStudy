#include "scene.h"

void Scene::categorizedStorage()
{
	m_vctSpotLights.clear();
	m_vctPointLights.clear();
	m_vctDirLights.clear();
	m_vctTransparentMeshs.clear();
	m_vctOpaqueMeshs.clear();

	std::queue<std::shared_ptr<ObjectBase>> queueObject;
	queueObject.push(shared_from_this());
	while (queueObject.size())
	{
		if (auto plight = std::dynamic_pointer_cast<SpotLight>(queueObject.front()))
		{
			m_vctSpotLights.push_back(plight);
		}

		if (auto plight = std::dynamic_pointer_cast<DirectionalLight>(queueObject.front()))
		{
			m_vctDirLights.push_back(plight);
		}

		if (auto plight = std::dynamic_pointer_cast<PointLight>(queueObject.front()))
		{
			m_vctPointLights.push_back(plight);
		}

		if (auto pMesh = std::dynamic_pointer_cast<Mesh>(queueObject.front()))
		{
			std::shared_ptr<Material> pMaterial = pMesh->getMaterial();
			if (pMaterial->m_bBlend)
			{
				m_vctTransparentMeshs.push_back(pMesh);
			}
			else
			{
				m_vctOpaqueMeshs.push_back(pMesh);
			}
		}

		for (auto obj : queueObject.front()->getChildren())
		{
			queueObject.push(obj);
		}

		queueObject.pop();
	}
}
