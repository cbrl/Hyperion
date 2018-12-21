template<typename EventT>
EventDispatcher<EventT>::~EventDispatcher() {
	//m_PendingAddDelegates.clear();
	pending_remove_delegates.clear();
	event_callbacks.clear();
}


template<typename EventT>
void EventDispatcher<EventT>::dispatch(IEvent* event) {
	locked = true;

	{
		// remove pending delegates
		if (pending_remove_delegates.empty() == false) {
			for (auto iter : pending_remove_delegates) {
				event_callbacks.erase(iter);
			}
			pending_remove_delegates.clear();
		}

		for (auto& callback : event_callbacks) {
			assert(callback != nullptr && "Invalid event callback");
			callback->invoke(event);
		}
	}

	locked = false;
}


template<typename EventT>
void EventDispatcher<EventT>::addEventCallback(IEventDelegate* delegate) {
	auto result = std::find_if(pending_remove_delegates.begin(), pending_remove_delegates.end(), [&](typename decltype(event_callbacks)::iterator& it) {
		return (*it)->operator==(*delegate);
	});

	// remove the delegate from the pending remove list if applicable
	if (result != pending_remove_delegates.end()) {
		pending_remove_delegates.erase(result);
	}
	else {
		event_callbacks.emplace_back(delegate);
	}
}


template<typename EventT>
void EventDispatcher<EventT>::removeEventCallback(IEventDelegate* delegate) {

	auto result = std::find_if(event_callbacks.begin(), event_callbacks.end(), [&](std::unique_ptr<IEventDelegate>& other) {
		return other->operator==(*delegate);
	});

	if (!locked && result != event_callbacks.end()) {
		event_callbacks.erase(result);
	}
	else {
		assert(result != event_callbacks.end() && "Invalid event callback specified for removal");
		pending_remove_delegates.push_back(result);
	}
}


template<typename EventT>
size_t EventDispatcher<EventT>::getEventCallbackCount() const {
	return event_callbacks.size();
}