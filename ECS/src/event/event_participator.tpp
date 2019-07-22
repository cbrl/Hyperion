#include "event_mgr.h"


namespace ecs {

//----------------------------------------------------------------------------------
// EventSender
//----------------------------------------------------------------------------------

template<typename EventT, typename... ArgsT>
void EventSender::enqueue(ArgsT&&... args) {
	static_assert(std::is_base_of_v<Event<EventT>, EventT>, "Event type must inherit from Event class");
	getEventMgr().enqueue<EventT>(std::forward<ArgsT>(args)...);
}




//----------------------------------------------------------------------------------
// EventListener
//----------------------------------------------------------------------------------

template<typename ClassT, typename EventT>
void EventListener::registerEventCallback(void (ClassT::*Callback)(const EventT&)) {
	static_assert(std::is_base_of_v<Event<EventT>, EventT>, "Event type must inherit from Event class");

	//ClassT inherits from EventListener
	std::function<void(const EventT&)> callback = std::bind(Callback, static_cast<ClassT*>(this), std::placeholders::_1);

	auto connection = getEventMgr().addCallback<EventT>(callback);
	connections.push_back(connection);
}

template<typename ClassT, typename EventT>
void EventListener::unregisterEventCallback(void (ClassT::*Callback)(const EventT&)) {
	static_assert(std::is_base_of_v<Event<EventT>, EventT>, "Event type must inherit from Event class");

	//ClassT inherits from EventListener
	std::function<void(const EventT&)> callback = std::bind(Callback, static_cast<ClassT*>(this), std::placeholders::_1);

	getEventMgr().removeCallback(callback);
}

} // namespace ecs