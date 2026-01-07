#include "scene.h"

void Scene::categorizedStorage()
{
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
			m_vctMeshs.push_back(pMesh);
		}

		for (auto obj : queueObject.front()->getChildren())
		{
			queueObject.push(obj);
		}

		queueObject.pop();
	}
}
