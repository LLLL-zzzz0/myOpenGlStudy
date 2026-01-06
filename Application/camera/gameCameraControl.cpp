#include "gameCameraControl.h"

GameCameraControl::GameCameraControl()
{
}

GameCameraControl::~GameCameraControl()
{
}

void GameCameraControl::onCursorEvent(const double& dXPos, const double& dYPos)
{
	float fDeltaX = (dXPos - m_dCurrentXpos) * m_fSensitivity;
	float fDeltaY = (dYPos - m_dCurrentYpos) * m_fSensitivity;

	if (m_bRightMouseDown)
	{
		pitch(-fDeltaY);
		yaw(-fDeltaX);
	}

	m_dCurrentXpos = dXPos;
	m_dCurrentYpos = dYPos;
}

void GameCameraControl::update()
{
	//最终移动方向
	glm::vec3 vec3Direction(0.0f);

	glm::vec3 vec3Front = glm::cross(m_pCamera->m_vec3Up, m_pCamera->m_vec3Right);
	glm::vec3 vec3Right = m_pCamera->m_vec3Right;

	if (m_hashMapKey.count(GLFW_KEY_W) && m_hashMapKey[GLFW_KEY_W])
	{
		vec3Direction += vec3Front;
	}

	if (m_hashMapKey.count(GLFW_KEY_S) && m_hashMapKey[GLFW_KEY_S])
	{
		vec3Direction -= vec3Front;
	}

	if (m_hashMapKey.count(GLFW_KEY_A) && m_hashMapKey[GLFW_KEY_A])
	{
		vec3Direction -= vec3Right;
	}

	if (m_hashMapKey.count(GLFW_KEY_D) && m_hashMapKey[GLFW_KEY_D])
	{
		vec3Direction += vec3Right;
	}

	//此时direction有可能不为1，也有可能是0
	if (glm::length(vec3Direction) != 0)
	{
		vec3Direction = glm::normalize(vec3Direction);
		m_pCamera->m_vec3Position += vec3Direction * m_fSpeed;
	}
}

void GameCameraControl::resetCamera()
{
	CameraControl::resetCamera();
	m_fPitch = 0.0f;
}

void GameCameraControl::pitch(float fAngle)
{
	m_fPitch += fAngle;
	if (m_fPitch > 89.0f || m_fPitch < -89.0f)
	{
		m_fPitch -= fAngle;
		return;
	}

	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(fAngle), m_pCamera->m_vec3Right);
	m_pCamera->m_vec3Up = rotateMat * glm::vec4(m_pCamera->m_vec3Up, 0.0f);
}

void GameCameraControl::yaw(float fAngle)
{
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(fAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	m_pCamera->m_vec3Up = rotateMat * glm::vec4(m_pCamera->m_vec3Up, 0.0f);
	m_pCamera->m_vec3Right = rotateMat * glm::vec4(m_pCamera->m_vec3Right, 0.0f);
}