#pragma once
#include <cstdint>
#include <functional>
#include <type_traits>

/*
All the different types of events will go in here
*/
enum class EventType : uint8_t
{
	None = 0,
	WindowClose, WindowResize,
	KeyPressed, KeyReleased
};


/*
Macros for easily adding get_static_method
*/
#define EVENT_TYPE(name) static const EventType get_static_type() { return EventType::name; } \
					virtual const EventType get_event_type() const noexcept override {return EventType::name; }

/*
Base class that can be used to describe some sort of event
*/
class Dispatcher;
class BasicEvent
{
	friend Dispatcher;
public:
	virtual ~BasicEvent() = default;

	bool _handled;

	virtual const EventType get_event_type() const noexcept { return EventType::None; };
	virtual const char* get_event_name() const noexcept { return "Event"; };

};


// Uses SPHINAE to make sure the class has a member function of get_static_type
template <typename T>
class StaticTypeMethod
{
private:
	typedef char YesType[1];
	typedef char NoType[2];

	template <typename C> static YesType& test(decltype(&C::get_static_type));
	template <typename C> static NoType& test(...);


public:
	enum { value = sizeof(test<T>(0)) == sizeof(YesType) };
};

/*
Used to run the callback only if the event matches what you want
*/
class Dispatcher
{
public:
	Dispatcher(BasicEvent& ev) noexcept
		:_event(ev)
	{}

	template<typename T, typename F>
	bool dispatch(const F& func) const
	{
		static_assert(StaticTypeMethod<T>::value, "You need to have the get_static_type method");
		
		if (_event.get_event_type() == T::get_static_type())
		{
			_event._handled = func(static_cast<T&>(_event));

			return true;
		}

		return false;
	}

private:
	BasicEvent& _event;
};