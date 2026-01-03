#pragma once
#include "camera.h"

class PerspectiveCamera :public Camera
{
public:
	PerspectiveCamera(const float& fFovy, const float& fAspect, const float& fNear, const float& fFar);
	~PerspectiveCamera();

	glm::mat4 getProjectionMatrix() override;

	void scale(float fDeltaScale) override;
private:
	float m_fFovy = 0.0f;
	float m_fAspect = 0.0f;
	float m_fNear = 0.0f;
	float m_fFar = 0.0f;
};