#pragma once
#include <cstdint>
#include <functional>
#include <type_traits>
#include <unordered_map>
#include <typeindex>

/*
All the different types of events will go in here
*/
enum class EventType : uint8_t
{
	None = 0,
	WindowClose, WindowResize,
	KeyPressed, KeyReleased,
	MouseMoved
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

	bool _handled = false;

	virtual const EventType get_event_type() const noexcept { return EventType::None; };
	virtual const char* get_event_name() const noexcept { return "Event"; };

};

/*
Responsible for handling the subscribers
This is a completely static class as there only should ever be 1 event
bus that handles all of the events for now at least
*/
class EventBus
{
public:
	using EventFn = std::function<bool(BasicEvent*)>;
	using EventVector = std::vector<EventFn>;

	template<typename EventType>
	static void publish(EventType* ev)
	{
		EventVector* handlers = _subscribers[typeid(EventType)];

		if (handlers != nullptr)
		{
			for (EventFn& func : *handlers)
			{
				if(func(ev)) break;
			}
		}
	}

	template<typename EventType>
	static void subscribe(EventFn func)
	{
		//get all of the callbacks for that event
		EventVector* handlers = _subscribers[typeid(EventType)];

		if (handlers == nullptr)
		{
			handlers = new EventVector();
			_subscribers[typeid(EventType)] = handlers;
		}

		handlers->emplace_back(func);
	}

private:
	static std::unordered_map<std::type_index, EventVector*> _subscribers;
};
