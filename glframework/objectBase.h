#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include "core.h"
#include "event/eventBus.h"

// ===============================
// Object Type
// ===============================
enum class ObjectType
{
	NODE,
	MESH,
	SCENE,
	LIGHT
};

// ===============================
// ObjectBase
// 只负责层级关系（Scene Graph）
// ===============================
class ObjectBase
	: public std::enable_shared_from_this<ObjectBase>
{
public:
	using Ptr = std::shared_ptr<ObjectBase>;
	virtual ~ObjectBase() = default;

	// RTTI（引擎级）
	virtual ObjectType getObjectType() const = 0;

	// 关键：提供 worldMatrix 接口
	virtual glm::mat4 getWorldMatrix() const
	{
		return glm::mat4(1.0f);
	}

	// ===== hierarchy =====
	const std::vector<Ptr>& getChildren() const { return m_children; }
	Ptr getParent() const { return m_parent.lock(); }

	// ===============================
	// setParent（最终版）
	// ===============================
	void setParent(const Ptr& newParent)
	{
		auto self = shared_from_this();
		auto oldParent = m_parent.lock();

		if (oldParent == newParent)
		{
			return;
		}

		// 从旧 parent 移除
		if (oldParent)
		{
			oldParent->removeChildInternal(self);
		}

		// 设置新 parent
		m_parent = newParent;

		// 加入新 parent
		if (newParent)
		{
			newParent->addChildInternal(self);
		}
		Event e;
		e.type = EventType::HierarchyChanged;
		e.sender = this;
		EventBus::instance().emit(e);

		// 通知派生类
		markDirtyRecursive();
	}

	virtual void markDirtyRecursive() {}

protected:

private:
	void addChildInternal(const Ptr& child)
	{
		if (std::find(m_children.begin(), m_children.end(), child) == m_children.end())
		{
			m_children.push_back(child);
		}
	}

	void removeChildInternal(const Ptr& child)
	{
		auto it = std::find(m_children.begin(), m_children.end(), child);
		if (it != m_children.end())
		{
			m_children.erase(it);
		}
	}

private:
	std::vector<Ptr> m_children;
	std::weak_ptr<ObjectBase> m_parent;
};
