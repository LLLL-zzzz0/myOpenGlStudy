#include "trackBallCameraControl.h"
#include <iostream>

TrackBallCameraControl::TrackBallCameraControl()
{
}

TrackBallCameraControl::~TrackBallCameraControl()
{
}

void TrackBallCameraControl::onCursorEvent(const double& dXPos, const double& dYPos)
{
	if (m_bLeftMouseDown)
	{
		float fDeltaX = (dXPos - m_dCurrentXpos) * m_fSensitivity;
		float fDeltaY = (dYPos - m_dCurrentYpos) * m_fSensitivity;

		pitch(-fDeltaY);
		yaw(-fDeltaX);

	}
	else if (m_bMiddleMouseDown)
	{
		float fDeltaX = (dXPos - m_dCurrentXpos) * m_fMoveSpeed;
		float fDeltaY = (dYPos - m_dCurrentYpos) * m_fMoveSpeed;

		m_pCamera->m_vec3Position += m_pCamera->m_vec3Up * fDeltaY;
		m_pCamera->m_vec3Position -= m_pCamera->m_vec3Right * fDeltaX;
	}
	m_dCurrentXpos = dXPos;
	m_dCurrentYpos = dYPos;
}

void TrackBallCameraControl::onScrollEvent(float fOffset)
{
	m_pCamera->scale(fOffset * m_fScaleSpeed);
}

void TrackBallCameraControl::pitch(float fAngle)
{
	//绕着m_vec3Right向量旋转
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(fAngle), m_pCamera->m_vec3Right);

	//影响当前相机的up向量和位置
	m_pCamera->m_vec3Up = rotateMat * glm::vec4(m_pCamera->m_vec3Up, 0.0f);
	m_pCamera->m_vec3Position = rotateMat * glm::vec4(m_pCamera->m_vec3Position, 1.0f);
}

void TrackBallCameraControl::yaw(float fAngle)
{
	//绕着m_vec3Right向量旋转
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(fAngle), glm::vec3(0.0f, 1.0f, 0.0f));

	//影响当前相机的up向量和位置
	m_pCamera->m_vec3Up = rotateMat * glm::vec4(m_pCamera->m_vec3Up, 0.0f);
	m_pCamera->m_vec3Right = rotateMat * glm::vec4(m_pCamera->m_vec3Right, 0.0f);
	m_pCamera->m_vec3Position = rotateMat * glm::vec4(m_pCamera->m_vec3Position, 1.0f);
}
