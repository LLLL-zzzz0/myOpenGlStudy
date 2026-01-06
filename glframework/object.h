#pragma once
#include "core.h"
#include <vector>
#include <memory>
#include <algorithm>

// ===============================
// CRTP Object Root
// ===============================
template<typename T>
class Object : public std::enable_shared_from_this<T>
{
public:
	using Ptr = std::shared_ptr<T>;
	
	// ===== 工厂：唯一创建方式 =====
	template<typename... Args>
	static Ptr Create(Args&&... args)
	{
		return Ptr(new T(std::forward<Args>(args)...));
	}

	virtual ~Object()
	{
	}

	void markDirty();

	// ===== Transform 接口 =====
	glm::mat4 getLocalMatrix() const;
	glm::mat4 getWorldMatrix() const;

	void setPosition(const glm::vec3& pos) 
	{
		if (m_vec3Position == pos)
		{
			return;
		}
		m_vec3Position = pos; 
		markDirty();
	}

	void setScale(const glm::vec3& scale) 
	{
		if (m_vec3Scale == scale)
		{
			return;
		}
		m_vec3Scale = scale; 
		markDirty();
	}

	glm::vec3 getPosition() const { return m_vec3Position; }

	void rotateX(float angle)
	{
		m_fAngleX += angle; 
		markDirty();
	}
	void rotateY(float angle)
	{
		m_fAngleY += angle;
		markDirty();
	}
	void rotateZ(float angle) 
	{
		m_fAngleZ += angle; 
		markDirty();
	}

	// ===== Hierarchy：唯一入口 =====
	void setParent(const Ptr& newParent)
	{
		auto self = std::static_pointer_cast<T>(this->shared_from_this());
		auto oldParent = m_pParent.lock();

		if (oldParent == newParent)
		{
			return;
		}

		if (oldParent)
		{
			oldParent->removeChildInternal(self);
		}

		m_pParent = newParent;

		if (newParent)
		{
			newParent->addChildInternal(self);
		}
		markDirty();
	}

	Ptr getParent() const
	{
		return m_pParent.lock();
	}

	const std::vector<Ptr>& getChildren() const
	{
		return m_vctChildren;
	}

protected:
	Object() = default;

	// ===== 仅内部使用 =====
	void addChildInternal(const Ptr& child)
	{
		auto it = std::find(m_vctChildren.begin(), m_vctChildren.end(), child);
		if (it == m_vctChildren.end())
			m_vctChildren.push_back(child);
	}

	void removeChildInternal(const Ptr& child)
	{
		auto it = std::find(m_vctChildren.begin(), m_vctChildren.end(), child);
		if (it != m_vctChildren.end())
			m_vctChildren.erase(it);
	}

private:
	// ===== Transform data =====
	float m_fAngleX{ 0.0f };
	float m_fAngleY{ 0.0f };
	float m_fAngleZ{ 0.0f };

	glm::vec3 m_vec3Scale{ 1.0f, 1.0f, 1.0f };
	glm::vec3 m_vec3Position{ 0.0f, 0.0f, 0.0f };

	// ===== Hierarchy =====
	std::vector<Ptr>      m_vctChildren;
	std::weak_ptr<T>     m_pParent;

	mutable bool      m_dirty{ true };
	mutable glm::mat4 m_cachedWorld{ 1.0f };
};

template<typename T>
glm::mat4 Object<T>::getLocalMatrix() const
{
	glm::mat4 transform(1.0f);

	transform = glm::scale(transform, m_vec3Scale);

	transform = glm::rotate(transform, glm::radians(m_fAngleX), glm::vec3(1, 0, 0));
	transform = glm::rotate(transform, glm::radians(m_fAngleY), glm::vec3(0, 1, 0));
	transform = glm::rotate(transform, glm::radians(m_fAngleZ), glm::vec3(0, 0, 1));

	transform = glm::translate(glm::mat4(1.0f), m_vec3Position) * transform;

	return transform;
}

template<typename T>
glm::mat4 Object<T>::getWorldMatrix() const
{
	if (!m_dirty)
	{
		return m_cachedWorld;
	}

	auto parent = m_pParent.lock();
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

template<typename T>
void Object<T>::markDirty()
{
	if (!m_dirty)
	{
		m_dirty = true;
		for (auto& child : m_vctChildren)
		{
			child->markDirty();
		}
	}
}
