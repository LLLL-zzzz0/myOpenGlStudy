#pragma once
#include "../core.h"

class Tools
{
public:
	//Ω‚ππæÿ’Û
	static void decomposeMat4(
		glm::mat4 mat4Matrix,
		glm::vec3& vec3Position,
		glm::vec3& vec3EulerAngle,
		glm::vec3& vec3Scale);
};