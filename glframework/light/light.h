#pragma once

#include "../core.h"
#include "../object.h"

enum class LightType
{
	Directional,
	Point,
	Spot
};

class Light:public Object<Light>
{
public:
	Light() = default;
	virtual ~Light() = default;
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

	virtual LightType getLightType() const = 0;

protected:
	explicit Light(LightType t)
		: Object(ObjectType::LIGHT), m_lightType(t) {
	}

	LightType m_lightType;
	glm::vec3 m_vec3Color{ 1.0f, 1.0f, 1.0f };
	float m_fSpecularIntensity{ 1.0f };
};