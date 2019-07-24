#include "event_mgr.h"


namespace ecs {

//----------------------------------------------------------------------------------
// EventSender
//----------------------------------------------------------------------------------

template<typename EventT, typename... ArgsT>
void EventSender::enqueue(ArgsT&&... args) {
	getEventMgr().enqueue<EventT>(std::forward<ArgsT>(args)...);
}

template<typename EventT, typename... ArgsT>
void EventSender::send(ArgsT&& ... args) {
	getEventMgr().send<EventT>(std::forward<ArgsT>(args)...);
}




//----------------------------------------------------------------------------------
// EventListener
//----------------------------------------------------------------------------------

template<typename ClassT, typename EventT>
void EventListener::registerEventCallback(void (ClassT::*Callback)(const EventT&)) {
	//ClassT inherits from EventListener
	std::function<void(const EventT&)> callback = std::bind(Callback, static_cast<ClassT*>(this), std::placeholders::_1);

	auto connection = getEventMgr().addCallback<EventT>(callback);
	connections.push_back(connection);
}

template<typename ClassT, typename EventT>
void EventListener::unregisterEventCallback(void (ClassT::*Callback)(const EventT&)) {
	//ClassT inherits from EventListener
	std::function<void(const EventT&)> callback = std::bind(Callback, static_cast<ClassT*>(this), std::placeholders::_1);

	getEventMgr().removeCallback(callback);
}

} // namespace ecs