namespace ecs {

template<typename EventT>
template<typename... ArgsT>
void EventDispatcher<EventT>::enqueue(ArgsT&&... args) {
	events[current_queue].emplace_back(std::forward<ArgsT>(args)...);
}


template<typename EventT>
template<typename... ArgsT>
void EventDispatcher<EventT>::send(ArgsT&&... args) {
	// Construct the event
	const EventT event{std::forward<ArgsT>(args)...};

	// Send the event
	for (auto& callback : event_callbacks) {
		callback(event);
	}
}


template<typename EventT>
void EventDispatcher<EventT>::dispatch() {
	// Update current_queue before processing events so that events sent during
	// event processing will be placed in the next queue.
	const u8 proc_queue = current_queue;
	current_queue = (current_queue + 1) % std::extent_v<decltype(events)>;

	// Dispatch events
	for (auto& callback : event_callbacks) {
		for (auto& event : events[proc_queue]) {
			callback(event);
		}
	}
	events[proc_queue].clear();
}


template<typename EventT>
DispatcherConnection EventDispatcher<EventT>::addCallback(const std::function<void(const EventT&)>& callback) {
	if (!callback)
		return {};

	// Search for an existing callback of the same type
	auto it = std::find_if(
		event_callbacks.begin(),
		event_callbacks.end(),
		[&](const std::function<void(const EventT&)>& other) {
			return other.target_type() == callback.target_type();
		}
	);

	// Add the callback if it doesn't exist
	if (it == event_callbacks.end()) {
		event_callbacks.push_back(callback);
		it = event_callbacks.begin() + (event_callbacks.size() - 1);
	}
	
	return DispatcherConnection{ [this, func = *it]{this->removeCallback(func);} };
}


template<typename EventT>
void EventDispatcher<EventT>::removeCallback(const std::function<void(const EventT&)>& callback) {
	if (!callback)
		return;

	event_callbacks.erase(
		std::remove_if(
			event_callbacks.begin(),
			event_callbacks.end(),
			[&callback](const std::function<void(const EventT&)>& other) {
				return other.target_type() == callback.target_type();
			}),
		event_callbacks.end()
	);
}


template<typename EventT>
size_t EventDispatcher<EventT>::getCallbackCount() const noexcept {
	return event_callbacks.size();
}

} // namespace ecs