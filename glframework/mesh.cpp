#include "mesh.h"

Mesh::Mesh(std::shared_ptr<Geometry> pGeometry, std::shared_ptr<Material> pMaterial)
{
	m_pGeometry = pGeometry;
	m_pMaterial = pMaterial;
}

Mesh::~Mesh()
{
}
