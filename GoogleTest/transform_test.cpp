#include <gtest/gtest.h>
#include "../glframework/node.h"

TEST(Transform, ParentTranslation)
{
	auto parent = Node::Create();
	auto child = Node::Create();

	parent->setPosition(glm::vec3(10, 0, 0));
	child->setPosition(glm::vec3(5, 0, 0));
	child->setParent(parent);

	glm::vec3 world = child->getWorldPosition();
	EXPECT_FLOAT_EQ(world.x, 15.0f);
}

TEST(Transform, ParentRotation)
{
	auto parent = Node::Create();
	auto child = Node::Create();

	parent->rotateZ(90.0f); // Z轴旋转90度
	child->setPosition(glm::vec3(1, 0, 0));
	child->setParent(parent);

	glm::vec3 world = child->getWorldPosition();

	// (1,0,0) 旋转90° 应该变成 (0,1,0)
	EXPECT_NEAR(world.x, 0.0f, 0.001f);
	EXPECT_NEAR(world.y, 1.0f, 0.001f);
}

TEST(Transform, ScalePropagation)
{
	auto parent = Node::Create();
	auto child = Node::Create();

	parent->setScale(glm::vec3(2, 2, 2));
	child->setPosition(glm::vec3(1, 0, 0));
	child->setParent(parent);

	glm::vec3 world = child->getWorldPosition();
	EXPECT_FLOAT_EQ(world.x, 2.0f);
}
