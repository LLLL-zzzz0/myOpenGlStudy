#include "tools.h"

void Tools::decomposeMat4(
	glm::mat4 mat4Matrix,
	glm::vec3& vec3Position,
	glm::vec3& vec3EulerAngle,
	glm::vec3& vec3Scale)
{
	//四元数，表示旋转变换
	glm::quat quaternion;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(mat4Matrix, vec3Scale, quaternion, vec3Position, skew, perspective);

	//四元数->欧拉角
	glm::mat4 rotation = glm::toMat4(quaternion);
	glm::extractEulerAngleXYX(rotation, vec3EulerAngle.x, vec3EulerAngle.y, vec3EulerAngle.z);

	//解构出来的角度是弧度,转化为角度
	vec3EulerAngle.x = glm::degrees(vec3EulerAngle.x);
	vec3EulerAngle.y = glm::degrees(vec3EulerAngle.y);
	vec3EulerAngle.z = glm::degrees(vec3EulerAngle.z);
}

