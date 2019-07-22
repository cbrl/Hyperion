#include "event/event_dispatcher.h"
#include "event/event.h"


namespace ecs {

template<typename EventT, typename... ArgsT>
void EventMgr::send(ArgsT&&... args) {
	static_assert(std::is_base_of_v<Event<EventT>, EventT>, "Event type must inherit from Event class");

	if (const auto it = event_dispatchers.find(EventT::static_index); it != event_dispatchers.end()) {
		auto* dispatcher = static_cast<EventDispatcher<EventT>*>(it->second.get());
		dispatcher->queue(std::forward<ArgsT>(args)...);
	}
}


template<typename EventT>
DispatcherConnection EventMgr::addEventCallback(const std::function<void(const EventT&)>& callback) {
	const auto index = EventT::static_index;
	const auto it    = event_dispatchers.find(index);

	if (it == event_dispatchers.end()) {
		const auto [iter, inserted] = event_dispatchers.try_emplace(index, std::make_unique<EventDispatcher<EventT>>());
		auto& dispatcher = static_cast<EventDispatcher<EventT>&>(*(iter->second));
		return dispatcher.addEventCallback(callback);
	}
	else {
		auto& dispatcher = static_cast<EventDispatcher<EventT>&>(*(it->second));
		return dispatcher.addEventCallback(callback);
	}
}


template<typename EventT>
void EventMgr::removeEventCallback(const std::function<void(const EventT&)>& callback) {
	if (const auto it = event_dispatchers.find(EventT::static_index); it != event_dispatchers.end()) {
		it->second->removeEventCallback(callback);
	}
}

} // namespace ecs