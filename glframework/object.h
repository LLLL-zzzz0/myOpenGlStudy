#pragma once

#include "objectBase.h"
#include <memory>
#include <algorithm>

// ===============================
// CRTP Object
// 只负责 Transform / Dirty
// ===============================
template<typename T>
class Object
	: public ObjectBase
{
public:
	virtual ~Object() = default;

	// ===== Transform =====
	glm::mat4 getLocalMatrix() const
	{
		glm::mat4 transform(1.0f);

		transform = glm::scale(transform, m_scale);
		transform = glm::rotate(transform, glm::radians(m_rotX), glm::vec3(1, 0, 0));
		transform = glm::rotate(transform, glm::radians(m_rotY), glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, glm::radians(m_rotZ), glm::vec3(0, 0, 1));
		transform = glm::translate(glm::mat4(1.0f), m_position) * transform;

		return transform;
	}

	glm::mat4 getWorldMatrix() const override
	{
		if (!m_dirty)
		{
			return m_cachedWorld;
		}

		auto parent = getParent();
		if (parent)
		{		
			m_cachedWorld = parent->getWorldMatrix() * getLocalMatrix();
		}
		else
		{
			m_cachedWorld = getLocalMatrix();
		}

		m_dirty = false;
		return m_cachedWorld;
	}

	void markDirtyRecursive() override
	{
		if (!m_dirty)
		{
			m_dirty = true;

			// 无类型递归：关键修复点
			for (auto& child : getChildren())
			{
				child->markDirtyRecursive();
			}
		}
	}


	// ===== setters =====
	void setPosition(const glm::vec3& pos)
	{
		if (m_position != pos)
		{
			m_position = pos;
			markDirtyRecursive();
		}
	}

	glm::vec3 getPosition() const { return m_position; }

	void setScale(const glm::vec3& scale)
	{
		if (m_scale != scale)
		{
			m_scale = scale;
			markDirtyRecursive();
		}
	}

	void rotateX(float angle) { m_rotX += angle; markDirtyRecursive(); }
	void rotateY(float angle) { m_rotY += angle; markDirtyRecursive(); }
	void rotateZ(float angle) { m_rotZ += angle; markDirtyRecursive(); }

protected:
	explicit Object(ObjectType type)
		: m_type(type)
	{
	}

public:
	ObjectType getObjectType() const override
	{
		return m_type;
	}

private:
	// ===== transform data =====
	float m_rotX{ 0.0f };
	float m_rotY{ 0.0f };
	float m_rotZ{ 0.0f };

	glm::vec3 m_scale{ 1.0f, 1.0f, 1.0f };
	glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };

	mutable bool      m_dirty{ true };
	mutable glm::mat4 m_cachedWorld{ 1.0f };

	ObjectType m_type;
};
