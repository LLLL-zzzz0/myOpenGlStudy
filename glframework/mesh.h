#pragma once

#include "object.h"
#include "geometry.h"
#include "material/material.h"
#include <memory>

class Mesh : public Object<Mesh>
{
public:
	using Ptr = std::shared_ptr<Mesh>;

	static Ptr Create(std::shared_ptr<Geometry> pGeometry, std::shared_ptr<Material> pMaterial)
	{
		return Ptr(new Mesh(pGeometry, pMaterial));
	}
	~Mesh();
	std::shared_ptr<Geometry> getGeometry() const { return m_pGeometry; }
	std::shared_ptr<Material> getMaterial() const { return m_pMaterial; }
protected:
	Mesh(std::shared_ptr<Geometry> pGeometry, std::shared_ptr<Material> pMaterial);

private:
	std::shared_ptr<Geometry> m_pGeometry;
	std::shared_ptr<Material> m_pMaterial;
};