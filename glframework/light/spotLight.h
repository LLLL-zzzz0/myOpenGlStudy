#pragma once

#include "light.h"
#include "../object.h"

class SpotLight :public Light, public Object<SpotLight>
{
public:
	friend class Object<SpotLight>;

	~SpotLight();
	void setTargetDirection(glm::vec3 vec3TargetDirection) { m_vec3TargetDirection = vec3TargetDirection; }
	glm::vec3 getTargetDirection()const { return m_vec3TargetDirection; }
	void setInnerAngle(const float& fAngle) { m_fInnerAngle = fAngle; }
	float getInnerAngle() { return m_fInnerAngle; }
	void setOuterAngle(const float& fAngle) { m_fOuterAngle = fAngle; }
	float getOuterAngle() { return m_fOuterAngle; }

protected:
	SpotLight() = default;
private:
	glm::vec3 m_vec3TargetDirection{-1.0f, -1.0f, -1.0f};
	float m_fInnerAngle{ 0.0f };
	float m_fOuterAngle{ 0.0f };
};

