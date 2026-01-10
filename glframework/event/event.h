#pragma once

enum class EventType
{
	HierarchyChanged,
	ObjectDestroyed
};

struct Event
{
	EventType type;
	void* sender;   // ObjectBase*
};
