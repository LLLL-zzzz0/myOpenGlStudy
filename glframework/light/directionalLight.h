#pragma once

#include "light.h"

class DirectionalLight :public Light
{
public:
	DirectionalLight();
	~DirectionalLight();
	void setDirection(glm::vec3 vec3Direction) 
	{
		m_vec3Direction = vec3Direction;
	}
	glm::vec3 getDirection() { return m_vec3Direction; }
private:
	glm::vec3 m_vec3Direction{ -1.0f, -1.0f, -1.0f };
};