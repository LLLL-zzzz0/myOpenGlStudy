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

		transform = glm::scale(transform, m_vec3Scale);
		transform = glm::rotate(transform, glm::radians(m_fRotX), glm::vec3(1, 0, 0));
		transform = glm::rotate(transform, glm::radians(m_fRotY), glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, glm::radians(m_fRotZ), glm::vec3(0, 0, 1));
		transform = glm::translate(glm::mat4(1.0f), m_vec3Position) * transform;

		return transform;
	}

	glm::mat4 getWorldMatrix() const override
	{
		if (!m_bDirty)
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

		m_bDirty = false;
		return m_cachedWorld;
	}

	void markDirtyRecursive() override
	{
		if (!m_bDirty)
		{
			m_bDirty = true;

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
		if (m_vec3Position != pos)
		{
			m_vec3Position = pos;
			markDirtyRecursive();
		}
	}

	glm::vec3 getPosition() const { return m_vec3Position; }

	glm::vec4 getWorldPosition() const
	{
		return glm::vec4(getWorldMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	glm::mat3 getNormalMatrix() const
	{
		return  glm::transpose(glm::inverse(glm::mat3(getWorldMatrix())));
	}

	glm::vec3 getWorldDirection() const
	{
		return glm::normalize(getNormalMatrix() * glm::vec3(0, 0, -1));
	}

	void setScale(const glm::vec3& scale)
	{
		if (m_vec3Scale != scale)
		{
			m_vec3Scale = scale;
			markDirtyRecursive();
		}
	}

	void rotateX(float fAngle) { m_fRotX += fAngle; markDirtyRecursive(); }
	void rotateY(float fAngle) { m_fRotY += fAngle; markDirtyRecursive(); }
	void rotateZ(float fAngle) { m_fRotZ += fAngle; markDirtyRecursive(); }

	void setAngleX(float fAngle) { m_fRotX = fAngle; }
	void setAngleY(float fAngle) { m_fRotY = fAngle; }
	void setAngleZ(float fAngle) { m_fRotZ = fAngle; }

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
	float m_fRotX{ 0.0f };
	float m_fRotY{ 0.0f };
	float m_fRotZ{ 0.0f };

	glm::vec3 m_vec3Scale{ 1.0f, 1.0f, 1.0f };
	glm::vec3 m_vec3Position{ 0.0f, 0.0f, 0.0f };

	mutable bool      m_bDirty{ true };
	mutable glm::mat4 m_cachedWorld{ 1.0f };

	ObjectType m_type;
};
