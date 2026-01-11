#include "node.h"

void Node::setModelBlend(ObjectBase* pObjectBase, bool bBlend, float opacity)
{
	if (pObjectBase->getObjectType() == ObjectType::MESH)
	{
		auto pMesh = static_cast<Mesh*>(pObjectBase);
		auto pMaterial = pMesh->getMaterial();
		pMaterial->m_bBlend = bBlend;
		pMaterial->m_fOpacity = opacity;
		pMaterial->m_bDepthWrite = false;
	}

	auto child = pObjectBase->getChildren();
	for (auto obj : child)
	{
		setModelBlend(obj.get(), bBlend, opacity);
	}
}
