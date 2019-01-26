#include "event_participator.h"
#include "event_mgr.h"


//----------------------------------------------------------------------------------
// EventParticipator
//----------------------------------------------------------------------------------

EventParticipator::EventParticipator() noexcept
    : event_mgr(nullptr) {
}


EventMgr& EventParticipator::getEventMgr() const noexcept {
	assert(event_mgr != nullptr && "EventParticipator::event_handler == nullptr");
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
		getEventMgr().removeEventCallback(callback);
	}

	registered_callbacks.clear();
}


void EventParticipator::setEventMgr(gsl::not_null<EventMgr*> handler) noexcept {
	event_mgr = handler;
}