#pragma once

#include "light.h"

class PointLight :public Light
{
public:
	using Ptr = std::shared_ptr<PointLight>;

	static Ptr Create()
	{
		return Ptr(new PointLight());
	}
	~PointLight() override = default;
	void setK2(const float& fK2) { m_fK2 = fK2; }
	void setK1(const float& fK1) { m_fK1 = fK1; }
	void setKc(const float& fKc) { m_fKc = fKc; }
	float getK2()const { return m_fK2; }
	float getK1()const { return m_fK1; }
	float getKc()const { return m_fKc; }

	LightType getLightType() const override
	{
		return LightType::Point;
	}

protected:
	PointLight()
		: Light(LightType::Point)
	{
	}

private:
	float m_fK2 = 1.0f;
	float m_fK1 = 1.0f;
	float m_fKc = 1.0f;
};