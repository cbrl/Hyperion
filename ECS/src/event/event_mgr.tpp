#include "event/event_dispatcher.h"
#include "event/event.h"


namespace ecs {

template<typename EventT, typename... ArgsT>
void EventMgr::enqueue(ArgsT&&... args) {
	static_assert(std::is_base_of_v<IEvent, EventT>, "Event type must inherit from Event class");

	if (const auto it = event_dispatchers.find(EventT::index); it != event_dispatchers.end()) {
		auto& dispatcher = static_cast<EventDispatcher<EventT>&>(*(it->second));
		dispatcher.enqueue(std::forward<ArgsT>(args)...);
	}
}


template<typename EventT, typename... ArgsT>
void EventMgr::send(ArgsT&& ... args) {
	static_assert(std::is_base_of_v<IEvent, EventT>, "Event type must inherit from Event class");

	if (const auto it = event_dispatchers.find(EventT::index); it != event_dispatchers.end()) {
		auto& dispatcher = static_cast<EventDispatcher<EventT>&>(*(it->second));
		dispatcher.send(std::forward<ArgsT>(args)...);
	}
}


template<typename EventT>
DispatcherConnection EventMgr::addCallback(const std::function<void(const EventT&)>& callback) {
	static_assert(std::is_base_of_v<IEvent, EventT>, "Event type must inherit from Event class");

	const auto index = EventT::index;
	const auto it    = event_dispatchers.find(index);

	if (it == event_dispatchers.end()) {
		const auto [iter, inserted] = event_dispatchers.try_emplace(index, std::make_unique<EventDispatcher<EventT>>());
		auto& dispatcher = static_cast<EventDispatcher<EventT>&>(*(iter->second));
		return dispatcher.addCallback(callback);
	}
	else {
		auto& dispatcher = static_cast<EventDispatcher<EventT>&>(*(it->second));
		return dispatcher.addCallback(callback);
	}
}


template<typename EventT>
void EventMgr::removeCallback(const std::function<void(const EventT&)>& callback) {
	static_assert(std::is_base_of_v<IEvent, EventT>, "Event type must inherit from Event class");

	if (const auto it = event_dispatchers.find(EventT::index); it != event_dispatchers.end()) {
		it->second->removeCallback(callback);
	}
}

} // namespace ecs