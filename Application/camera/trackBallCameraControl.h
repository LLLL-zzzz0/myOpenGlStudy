#pragma once
#include "cameraControl.h"

class TrackBallCameraControl :public CameraControl
{
public:
	TrackBallCameraControl();
	~TrackBallCameraControl();

	void onCursorEvent(const double& dXPos, const double& dYPos)override;
	void onScrollEvent(float fOffset)override;
private:
	void pitch(float fAngle);
	void yaw(float fAngle);

private:
	float m_fMoveSpeed = 0.01f;
};