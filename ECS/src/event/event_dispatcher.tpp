template<typename EventT>
EventDispatcher<EventT>::EventDispatcher()
    : delegate_list_locked(false) {
}

template<typename EventT>
EventDispatcher<EventT>::~EventDispatcher() {
	//m_PendingAddDelegates.clear();
	pending_remove_delegates.clear();
	event_delegates.clear();
}


template<typename EventT>
void EventDispatcher<EventT>::dispatch(IEvent* event) {
	delegate_list_locked = true;

	// Remove pending delegates
	if (!pending_remove_delegates.empty()) {
		for (auto& it : pending_remove_delegates) {
			event_delegates.erase(it);
		}
		pending_remove_delegates.clear();
	}

	// Dispatch event
	for (auto& delegate : event_delegates) {
		assert(delegate != nullptr && "Invalid event callback");
		delegate->invoke(event);
	}

	delegate_list_locked = false;
}


template<typename EventT>
void EventDispatcher<EventT>::addEventCallback(IEventDelegate* delegate) {
	auto result = std::find_if(pending_remove_delegates.begin(), pending_remove_delegates.end(),
		[&](typename decltype(event_delegates)::iterator& it) {
			return (*it)->operator==(*delegate);
	});

	// remove the delegate from the pending remove list if applicable
	if (result != pending_remove_delegates.end()) {
		pending_remove_delegates.erase(result);
	}
	else {
		event_delegates.emplace_back(delegate);
	}
}


template<typename EventT>
void EventDispatcher<EventT>::removeEventCallback(IEventDelegate* delegate) {

	auto result = std::find_if(event_delegates.begin(), event_delegates.end(), [&](std::unique_ptr<IEventDelegate>& other) {
		return other->operator==(*delegate);
	});

	if (!delegate_list_locked && result != event_delegates.end()) {
		event_delegates.erase(result);
	}
	else {
		assert(result != event_delegates.end() && "Invalid event callback specified for removal");
		pending_remove_delegates.push_back(result);
	}
}


template<typename EventT>
size_t EventDispatcher<EventT>::getEventCallbackCount() const {
	return event_delegates.size();
}