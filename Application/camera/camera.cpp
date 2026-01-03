#include "camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::getViewMatrix()
{
	//lookat
	//eye: 相机位置
	//center: 看向世界坐标的哪个点
	//up: 穹顶向量
	glm::vec3 vec3Front = glm::normalize(glm::cross(m_vec3Up, m_vec3Right));
	glm::vec3 vec3Center = m_vec3Position + vec3Front;
	return glm::lookAt(m_vec3Position, vec3Center, m_vec3Up);
}

glm::mat4 Camera::getProjectionMatrix()
{
	return glm::mat4(1.0f);
}

void Camera::scale(float fDeltaScale)
{
}
