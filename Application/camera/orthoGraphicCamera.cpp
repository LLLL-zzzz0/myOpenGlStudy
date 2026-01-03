#include "orthoGraphicCamera.h"

OrthoGraphicCamera::OrthoGraphicCamera(const float& fLeft, const float& fRight, const float& fTop, const float& fBottom, const float& fNear, const float& fFar)
{
	m_fLeft = fLeft;
	m_fRight = fRight;
	m_fTop = fTop;
	m_fBottom = fBottom;
	m_fNear = fNear;
	m_fFar = fFar;
}

OrthoGraphicCamera::~OrthoGraphicCamera()
{
}

glm::mat4 OrthoGraphicCamera::getProjectionMatrix()
{
	float fScale = std::pow(2.0f, m_fScale);
	return glm::ortho(m_fLeft * fScale, m_fRight * fScale, m_fBottom * fScale, m_fTop * fScale, m_fNear, m_fFar);
}

void OrthoGraphicCamera::scale(float fDeltaScale)
{
	m_fScale += fDeltaScale;
}
