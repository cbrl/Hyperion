#include "event/event_dispatcher.h"


namespace ecs {

template<typename EventT, typename... ArgsT>
void EventMgr::enqueue(ArgsT&&... args) {
	const std::type_index index{typeid(EventT)};

	if (const auto it = event_dispatchers.find(index); it != event_dispatchers.end()) {
		auto& dispatcher = static_cast<EventDispatcher<EventT>&>(*(it->second));
		dispatcher.enqueue(std::forward<ArgsT>(args)...);
	}
}


template<typename EventT, typename... ArgsT>
void EventMgr::send(ArgsT&&... args) {
	const std::type_index index{typeid(EventT)};

	if (const auto it = event_dispatchers.find(index); it != event_dispatchers.end()) {
		auto& dispatcher = static_cast<EventDispatcher<EventT>&>(*(it->second));
		dispatcher.send(std::forward<ArgsT>(args)...);
	}
}


template<typename EventT>
EventDispatcher<EventT>& EventMgr::getDispatcher() {
	return getOrCreateDispatcher<EventT>();
}


template<typename EventT>
EventDispatcher<EventT>& EventMgr::getOrCreateDispatcher() {
	const std::type_index index{typeid(EventT)};
	const auto it = event_dispatchers.find(index);

	if (it == event_dispatchers.end()) {
		const auto [iter, inserted] = event_dispatchers.try_emplace(index, std::make_unique<EventDispatcher<EventT>>());
		return static_cast<EventDispatcher<EventT>&>(*(iter->second));
	}
	else {
		return static_cast<EventDispatcher<EventT>&>(*(it->second));
	}
}


template<typename EventT>
void EventMgr::removeCallback(const std::function<void(const EventT&)>& callback) {
	const std::type_index index{typeid(EventT)};

	if (const auto it = event_dispatchers.find(index); it != event_dispatchers.end()) {
		it->second->removeCallback(callback);
	}
}

} // namespace ecs