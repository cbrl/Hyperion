template <class EventT, class... ArgsT>
void EventHandler::send(ArgsT&&... args) {

	static_assert(std::is_base_of_v<Event<EventT>, EventT>, "Event type must inherit from Event class");

	// allocate memory to store event data
	void* mem = nullptr;
	try {
		// add new event to buffer and event storage
		mem = event_pool.allocate(sizeof(EventT), alignof(EventT));
		events.emplace_back(new (mem) EventT(std::forward<ArgsT>(args)...));
	}
	catch (const std::exception& e) {
		Logger::log(LogLevel::err, "Could not allocate memory for event: ", e.what());
	}
}


template <class EventT>
void EventHandler::addEventCallback(IEventDelegate* delegate) {
	auto index = EventT::static_index;

	auto it = event_dispatchers.find(index);
	if (it == event_dispatchers.end()) {
		auto pair = event_dispatchers.try_emplace(index, std::make_unique<EventDispatcher<EventT>>());
		pair.first->second->addEventCallback(delegate);
	}
	else {
		event_dispatchers[index]->addEventCallback(delegate);
	}
}