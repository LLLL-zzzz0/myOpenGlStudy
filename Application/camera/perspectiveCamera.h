#pragma once
#include "camera.h"

class PerspectiveCamera :public Camera
{
public:
	PerspectiveCamera(const float& fFovy, const float& fAspect, const float& fNear, const float& fFar);
	~PerspectiveCamera();

	glm::mat4 getProjectionMatrix() override;

	void scale(float fDeltaScale) override;
	float getNear() override { return m_fNear; }
	float getFar() override { return m_fFar; }
private:
	float m_fFovy = 0.0f;
	float m_fAspect = 0.0f;
	float m_fNear = 0.0f;
	float m_fFar = 0.0f;
};