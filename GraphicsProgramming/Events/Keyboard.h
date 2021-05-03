#pragma once
#include "Event.h"

class KeyPressedEvent : public BasicEvent
{
public:
	KeyPressedEvent(int key_code) noexcept
		:_key_code{key_code}
	{};

	const int& get_code() const noexcept { return _key_code; };

	EVENT_TYPE(KeyPressed);

private:
	int _key_code;
};

class KeyReleasedEvent : public BasicEvent
{
public:
	KeyReleasedEvent(int key_code) noexcept
		:_key_code{ key_code }
	{};

	const int& get_code() const noexcept { return _key_code; };

	EVENT_TYPE(KeyPressed);

private:
	int _key_code;
};
