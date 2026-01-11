#pragma once

#include "light.h"

class DirectionalLight :public Light
{
public:
	using Ptr = std::shared_ptr<DirectionalLight>;

	static Ptr Create()
	{
		return Ptr(new DirectionalLight());
	}

	~DirectionalLight();
	LightType getLightType() const override
	{
		return LightType::Directional;
	}

	glm::vec3 getWorldDirection() const
	{
		return glm::normalize(glm::vec3(getWorldMatrix() * glm::vec4(0, 0, -1, 0)));
	}

protected:
	DirectionalLight():
		Light(LightType::Directional)
	{
	}
private:
	
};