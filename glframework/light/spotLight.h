#pragma once

#include "light.h"
#include <iostream>

class SpotLight :public Light
{
public:
	using Ptr = std::shared_ptr<SpotLight>;

	static Ptr Create()
	{
		return Ptr(new SpotLight());
	}
	~SpotLight();
	void setInnerAngle(const float& fAngle) { m_fInnerAngle = fAngle; }
	float getInnerAngle() { return m_fInnerAngle; }
	void setOuterAngle(const float& fAngle) { m_fOuterAngle = fAngle; }
	float getOuterAngle() { return m_fOuterAngle; }

	LightType getLightType() const override
	{
		return LightType::Spot;
	}

protected:
	SpotLight() : 
		Light(LightType::Spot)
	{
	}

private:
	float m_fInnerAngle{ 0.0f };
	float m_fOuterAngle{ 0.0f };
};

