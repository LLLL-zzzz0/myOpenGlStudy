#pragma once

#include "object.h"
#include "geometry.h"
#include "material/material.h"
#include <memory>

class Mesh : public Object<Mesh>
{
public:
	Mesh(std::shared_ptr<Geometry> pGeometry, std::shared_ptr<Material> pMaterial);
	~Mesh();
	Geometry* getGeometry() {return m_pGeometry.get();}
	Material* getMaterial() { return m_pMaterial.get(); }

private:
	std::shared_ptr<Geometry> m_pGeometry;
	std::shared_ptr<Material> m_pMaterial;
};