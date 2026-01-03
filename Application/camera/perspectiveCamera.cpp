#include "perspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(const float& fFovy, const float& fAspect, const float& fNear, const float& fFar)
{
	m_fFovy = fFovy;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;
}

PerspectiveCamera::~PerspectiveCamera()
{
}

glm::mat4 PerspectiveCamera::getProjectionMatrix()
{
	return glm::perspective(glm::radians(m_fFovy), m_fAspect, m_fNear, m_fFar);
}

void PerspectiveCamera::scale(float fDeltaScale)
{
	glm::vec3 vec3Front = glm::cross(m_vec3Up, m_vec3Right);
	m_vec3Position += (vec3Front * fDeltaScale);
}
