#pragma once
#include "Event.h"
#include "../utils/Vectors.h"


class WindowCloseEvent : public BasicEvent
{
public:
	WindowCloseEvent() noexcept
	{}
	EVENT_TYPE(WindowClose);
};

class WindowResizeEvent : public BasicEvent
{

public:
	WindowResizeEvent(int w, int h) noexcept
		:size{w, h}
	{}

	const Vec2<int>& get_size() const noexcept { return size;  }

	EVENT_TYPE(WindowResize);

private:
	Vec2<int> size;
};