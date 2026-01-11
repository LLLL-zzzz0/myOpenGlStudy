#include <gtest/gtest.h>
#include "../glframework/light/light.h"
#include "../glframework/light/pointLight.h"
#include "../glframework/light/spotLight.h"

TEST(Light, Color)
{
	auto light = PointLight::Create();
	light->setColor(glm::vec3(1, 0.5f, 0.25f));

	glm::vec3 c = light->getColor();
	EXPECT_FLOAT_EQ(c.x, 1.0f);
	EXPECT_FLOAT_EQ(c.y, 0.5f);
	EXPECT_FLOAT_EQ(c.z, 0.25f);
}

TEST(SpotLight, DirectionNormalization)
{
	auto light = SpotLight::Create();
	light->setPosition(glm::vec3(10, 0, 0));

	glm::vec3 dir = light->getWorldDirection();

	float len = glm::length(dir);
	EXPECT_NEAR(len, 1.0f, 0.001f);
}
