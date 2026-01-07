#pragma once
#include "../core.h"

class AmbientLight
{
public:
	AmbientLight();
	~AmbientLight();
	glm::vec3 getColor() { return m_vec3Color; }
	void setColor(glm::vec3 vec3Color)
	{
		m_vec3Color = vec3Color;
	}

private:
	glm::vec3 m_vec3Color{ 0.1f };
};