#pragma once

#include "../core.h"

class Light
{
public:
	Light();
	~Light();
	void setColor(glm::vec3 vec3Color) 
	{
		m_vec3Color = vec3Color;
	}
	void setSpecularIntensity(float fSpecularIntensity)
	{
		m_fSpecularIntensity = fSpecularIntensity;
	}
	glm::vec3 getColor() { return m_vec3Color; }
	float getSpecularIntensity() { return m_fSpecularIntensity; }

protected:
	glm::vec3 m_vec3Color{ 1.0f, 1.0f, 1.0f };
	float m_fSpecularIntensity{ 1.0f };
};