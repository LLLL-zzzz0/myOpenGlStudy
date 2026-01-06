#pragma once
#include "core.h"
#include <vector>
#include <memory>
#include <map>

class Object:public std::enable_shared_from_this<Object>
{
public:

	// Object 必须通过 shared_ptr 创建
	static std::shared_ptr<Object> Create()
	{
		return std::make_shared<Object>();
	}

	Object();
	virtual ~Object();

	glm::mat4 getModelMatrix();
	void setPosition(glm::vec3 vec3Pos);
	void setScale(glm::vec3 vec3Scale);
	glm::vec3 getPosition()const { return m_vec3Position; }
	//增量旋转
	void rotateX(float fAngle);
	void rotateY(float fAngle);
	void rotateZ(float fAngle);

	//父子关系
	const std::vector<std::shared_ptr<Object>>& getChildren();
	void setParent(std::shared_ptr<Object> pParent);
	std::weak_ptr<Object> getParent()const;

protected:

	void Object::addChildInternal(std::shared_ptr<Object> child)
	{
		auto it = std::find(m_vctChildren.begin(), m_vctChildren.end(), child);
		if (it == m_vctChildren.end())
		{
			m_vctChildren.push_back(child);
		}
	}

	void Object::removeChildInternal(std::shared_ptr<Object> child)
	{
		auto it = std::find(m_vctChildren.begin(), m_vctChildren.end(), child);
		if (it != m_vctChildren.end())
		{
			m_vctChildren.erase(it);
		}
	}

	//unity 旋转标准: pitch yaw roll
	float m_fAngleX{ 0.0f };
	float m_fAngleY{0.0f };
	float m_fAngleZ{ 0.0f };

	glm::vec3 m_vec3Scale{1.0f, 1.0f, 1.0f};
	glm::vec3 m_vec3Position{ 0.0f, 0.0f, 0.0f };

	//父子关系
	std::vector<std::shared_ptr<Object>> m_vctChildren;
	std::weak_ptr<Object> m_pParent;
};