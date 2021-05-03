#pragma once
#include "Event.h"
#include "../utils/Vectors.h"

class MouseMovedEvent : public BasicEvent
{
public:
	MouseMovedEvent(double xpos, double ypos) noexcept
		:_pos{xpos, ypos}
	{};

	const Vec2<double>& get_pos() const noexcept { return _pos; };

	EVENT_TYPE(MouseMoved);

private:
	Vec2<double> _pos;
};
