
namespace ecs {

template<typename EventT>
EventDispatcher<EventT>::EventDispatcher()
    : delegate_list_locked(false) {
}

template<typename EventT>
EventDispatcher<EventT>::~EventDispatcher() {
	//pending_add_delegates.clear();
	pending_remove_delegates.clear();
	event_delegates.clear();
}


template<typename EventT>
void EventDispatcher<EventT>::dispatch(const IEvent& event) {
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
void EventDispatcher<EventT>::addEventCallback(std::unique_ptr<IEventDelegate> delegate) {
	if (!delegate)
		return;

	// Search for an existing delegate of the same type in the pending remove list
	const auto result = std::find_if(
		pending_remove_delegates.begin(),
		pending_remove_delegates.end(),
		[&](typename decltype(event_delegates)::iterator& it) {
			return (*it)->operator==(*delegate);
		}
	);

	// If the above search found a match, just remove it from the pending remove list.
	if (result != pending_remove_delegates.end()) {
		pending_remove_delegates.erase(result);
	}
	else { //otherwise add the delegate to the delegate list
		event_delegates.emplace_back(std::move(delegate));
	}
}


template<typename EventT>
void EventDispatcher<EventT>::removeEventCallback(gsl::not_null<IEventDelegate*> delegate) {

	// Look for a matching delegate in the delegate list
	const auto result = std::find_if(
		event_delegates.begin(),
		event_delegates.end(),
		[&](std::unique_ptr<IEventDelegate>& other) {
			return other->operator==(*delegate);
		}
	);

	assert(result != event_delegates.end() && "Invalid event callback specified for removal");

	// Remove the delegate
	if (result != event_delegates.end()) {
		if (!delegate_list_locked) {
			event_delegates.erase(result);
		}
		else {
			pending_remove_delegates.push_back(result);
		}
	}
}


template<typename EventT>
size_t EventDispatcher<EventT>::getEventCallbackCount() const noexcept {
	return event_delegates.size();
}

} // namespace ecs