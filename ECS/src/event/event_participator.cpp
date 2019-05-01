#include "event_participator.h"
#include "event_mgr.h"


namespace ecs {

//----------------------------------------------------------------------------------
// EventParticipator
//----------------------------------------------------------------------------------

void EventParticipator::setEventMgr(gsl::not_null<EventMgr*> handler) noexcept {
	event_mgr = handler;
}

EventMgr& EventParticipator::getEventMgr() const noexcept {
	assert(event_mgr != nullptr &&
	       "EventParticipator::event_handler == nullptr (Was sendEvent() called in a component/system constructor?)");
	return *event_mgr;
}




//----------------------------------------------------------------------------------
// EventListener
//----------------------------------------------------------------------------------

EventListener::~EventListener() {
	unregisterAllEventCallbacks();
}

void EventListener::unregisterAllEventCallbacks() {

	// Unsubcribe from all events
	for (auto* callback : registered_callbacks) {
		if (callback)
			getEventMgr().removeEventCallback(gsl::make_not_null(callback));
	}

	registered_callbacks.clear();
}

void EventListener::doRegisterCallbacks() {
	registerCallbacks();
}

} // namespace ecs