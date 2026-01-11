#include "scene.h"

void Scene::categorizedStorage()
{
	m_vctSpotLights.clear();
	m_vctPointLights.clear();
	m_vctDirLights.clear();
	m_vctTransparentMeshs.clear();
	m_vctOpaqueMeshs.clear();

	categorizedStorage(shared_from_this().get());
}

void Scene::categorizedStorage(ObjectBase* pObject)
{
	switch (pObject->getObjectType())
	{
	case ObjectType::LIGHT:
	{
		auto light = static_cast<Light*>(pObject);
		switch (light->getLightType())
		{
		case LightType::Spot:
			m_vctSpotLights.push_back(static_cast<SpotLight*>(light));
			break;
		case LightType::Directional:
			m_vctDirLights.push_back(static_cast<DirectionalLight*>(light));
			break;
		case LightType::Point:
			m_vctPointLights.push_back(static_cast<PointLight*>(light));
			break;
		}
		break;
	}
	case ObjectType::MESH:
	{
		auto mesh = static_cast<Mesh*>(pObject);
		auto mat = mesh->getMaterial();
		if (mat->m_bBlend)
			m_vctTransparentMeshs.push_back(mesh);
		else
			m_vctOpaqueMeshs.push_back(mesh);
		break;
	}
	default:
		break;
	}

	for (auto obj : pObject->getChildren())
	{
		categorizedStorage(obj.get());
	}
}
