#pragma once
#include "object.h"
#include "mesh.h"

class Node : public Object<Node>
{
public:
	using Ptr = std::shared_ptr<Node>;

	static Ptr Create()
	{
		return Ptr(new Node());
	}

	void setModelBlend(bool bBlend, float fOpacity = 1.0f)
	{
		setModelBlend(shared_from_this().get(), bBlend, fOpacity);
	}

protected:
	void setModelBlend(ObjectBase* pObjectBase, bool bBlend, float fOpacity = 1.0f);
	Node() :Object(ObjectType::NODE)
	{
	}
};