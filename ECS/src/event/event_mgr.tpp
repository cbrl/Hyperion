
namespace ecs {

template <class EventT, class... ArgsT>
void EventMgr::send(ArgsT&&... args) {

	static_assert(std::is_base_of_v<Event<EventT>, EventT>, "Event type must inherit from Event class");

	// allocate memory to store event data
	EventT* mem = nullptr;
	try {
		// add new event to buffer and event storage
		mem = static_cast<EventT*>(event_pool.allocate(sizeof(EventT), alignof(EventT)));
		new(mem) EventT(std::forward<ArgsT>(args)...);
		events.push_back(mem);
	}
	catch (const std::exception& e) {
		Logger::log(LogLevel::err, "Could not allocate memory for event: ", e.what());
	}
}


template <class EventT>
void EventMgr::addEventCallback(std::unique_ptr<IEventDelegate> delegate) {
	const auto index = EventT::static_index;
	const auto it    = event_dispatchers.find(index);

	if (it == event_dispatchers.end()) {
		const auto pair = event_dispatchers.try_emplace(index, std::make_unique<EventDispatcher<EventT>>());
		pair.first->second->addEventCallback(std::move(delegate));
	}
	else {
		it->second->addEventCallback(std::move(delegate));
	}
}

} // namespace ecs