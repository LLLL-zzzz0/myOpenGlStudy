#pragma once

#include "cameraControl.h"

class GameCameraControl :public CameraControl
{
public:
	GameCameraControl();
	~GameCameraControl();

	void onCursorEvent(const double& dXPos, const double& dYPos)override;
	void update()override;
	void setSpeed(const float& fSpeed) { m_fSpeed = fSpeed; }
private:
	void pitch(float fAngle);
	void yaw(float fAngle);

	float m_fPitch{ 0.0f };
	float m_fSpeed{ 0.001f };
};