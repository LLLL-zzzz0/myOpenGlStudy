#pragma once
#include "Event.h"

class EventListener
{
public:
	virtual ~EventListener() = default;
	virtual void onEvent(const Event& e) = 0;
};
