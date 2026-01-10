#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <algorithm>
#include "Event.h"
#include "EventListener.h"

class EventBus
{
public:
	static EventBus& instance()
	{
		static EventBus bus;
		return bus;
	}

	// ============================
	// subscribe（弱引用）
	// ============================
	void subscribe(EventType type, std::shared_ptr<EventListener> listener)
	{
		m_listeners[type].push_back(listener);
	}

	// ============================
	// emit（自动清理失效 listener）
	// ============================
	void emit(const Event& e)
	{
		auto it = m_listeners.find(e.type);
		if (it == m_listeners.end())
			return;

		auto& vec = it->second;

		for (auto iter = vec.begin(); iter != vec.end(); )
		{
			if (auto sp = iter->lock())
			{
				sp->onEvent(e);
				++iter;
			}
			else
			{
				// listener 已销毁，自动清理
				iter = vec.erase(iter);
			}
		}
	}

private:
	using WeakListener = std::weak_ptr<EventListener>;
	std::unordered_map<EventType, std::vector<WeakListener>> m_listeners;
};
