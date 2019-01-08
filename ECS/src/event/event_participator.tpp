#include "event_mgr.h"


//----------------------------------------------------------------------------------
// EventSender
//----------------------------------------------------------------------------------

template <typename EventT, typename... ArgsT>
void EventSender::sendEvent(ArgsT&&... args) {
	static_assert(std::is_base_of_v<Event<EventT>, EventT>, "Event type must inherit from Event class");
	getEventMgr().send<EventT>(std::forward<ArgsT>(args)...);
}




//----------------------------------------------------------------------------------
// EventListener
//----------------------------------------------------------------------------------

template <typename ClassT, typename EventT>
void EventListener::registerEventCallback(void (ClassT::*Callback)(const EventT* const)) {

	static_assert(std::is_base_of_v<Event<EventT>, EventT>, "Event type must inherit from Event class");

	// Create a new delegate. The event manager will own the delegate.
	IEventDelegate* delegate = new EventDelegate<ClassT, EventT>(static_cast<ClassT*>(this), Callback); //ClassT inherits from "this"

	auto result = std::find_if(registered_callbacks.begin(), registered_callbacks.end(), [&](const IEventDelegate* other) {
		return other->operator==(*delegate);
	});

	if (result == registered_callbacks.end()) {
		registered_callbacks.push_back(delegate);
		getEventMgr().addEventCallback<EventT>(delegate);
	}
}

template <typename ClassT, typename EventT>
void EventListener::unregisterEventCallback(void (ClassT::*Callback)(const EventT* const)) {

	static_assert(std::is_base_of_v<Event<EventT>, EventT>, "Event type must inherit from Event class");

	EventDelegate<ClassT, EventT> cmp_delegate(static_cast<ClassT*>(this), Callback); //ClassT inherits from "this"

	auto result = std::find_if(registered_callbacks.begin(), registered_callbacks.end(), [&](const IEventDelegate* other) {
		return other->operator==(cmp_delegate);
	});

	if (result != registered_callbacks.end()) {
		getEventMgr().removeEventCallback(&(*result));
		registered_callbacks.erase(result);
	}
}