#include "event_mgr.h"


namespace ecs {

//----------------------------------------------------------------------------------
// EventSender
//----------------------------------------------------------------------------------

template<typename EventT, typename... ArgsT>
void EventSender::sendEvent(ArgsT&&... args) {
	static_assert(std::is_base_of_v<Event<EventT>, EventT>, "Event type must inherit from Event class");
	getEventMgr().send<EventT>(std::forward<ArgsT>(args)...);
}




//----------------------------------------------------------------------------------
// EventListener
//----------------------------------------------------------------------------------

template<typename ClassT, typename EventT>
void EventListener::registerEventCallback(void (ClassT::*Callback)(const EventT&)) {

	static_assert(std::is_base_of_v<Event<EventT>, EventT>, "Event type must inherit from Event class");

	// Create a new delegate. The event manager will own the delegate.
	auto delegate = std::make_unique<EventDelegate<ClassT, EventT>>(static_cast<ClassT*>(this), Callback); //ClassT inherits from "this"

	// Search the registered callbacks for a callback matching the new one
	const auto result = std::find_if(
		registered_callbacks.begin(),
		registered_callbacks.end(),
		[&](const IEventDelegate* other) {
			return other->operator==(*delegate);
		}
	);

	// Add the callback if it doesn't exist
	if (result == registered_callbacks.end()) {
		registered_callbacks.push_back(delegate.get());
		getEventMgr().addEventCallback<EventT>(std::move(delegate));
	}
}

template<typename ClassT, typename EventT>
void EventListener::unregisterEventCallback(void (ClassT::*Callback)(const EventT&)) {

	static_assert(std::is_base_of_v<Event<EventT>, EventT>, "Event type must inherit from Event class");

	EventDelegate<ClassT, EventT> cmp_delegate(static_cast<ClassT*>(this), Callback); //ClassT inherits from "this"

	// Search the registered callbacks for a match
	const auto result = std::find_if(
		registered_callbacks.begin(),
		registered_callbacks.end(), 
		[&](const IEventDelegate* other) {
			return other->operator==(cmp_delegate);
		}
	);

	// Remove the callback
	if (result != registered_callbacks.end()) {
		getEventMgr().removeEventCallback(gsl::make_not_null(&(*result)));
		registered_callbacks.erase(result);
	}
}

} // namespace ecs