#pragma once
#include "camera.h"

class OrthoGraphicCamera :public Camera
{
public:
	OrthoGraphicCamera(const float& fLeft, const float& fRight, const float& fTop, const float& fBottom, const float& fNear, const float& fFar);
	~OrthoGraphicCamera();

	glm::mat4 getProjectionMatrix() override;

	void scale(float fDeltaScale) override;
private:
	float m_fLeft = 0.0f;
	float m_fRight = 0.0f;
	float m_fTop = 0.0f;
	float m_fBottom = 0.0f;
	float m_fNear = 0.0f;
	float m_fFar = 0.0f;

	float m_fScale = 0.0f;
};