#include "event_participator.h"
#include "event_handler.h"


//----------------------------------------------------------------------------------
// EventParticipator
//----------------------------------------------------------------------------------

EventHandler* EventParticipator::getEventHandler() const noexcept {
	assert(event_handler != nullptr && "EventParticipator::event_handler == nullptr");
	return event_handler;
}




//----------------------------------------------------------------------------------
// EventListener
//----------------------------------------------------------------------------------

EventListener::~EventListener() {
	// unsubcribe from all subscribed events
	unregisterAllEventCallbacks();
}


void EventListener::unregisterAllEventCallbacks() {

	// unsubcribe from all subscribed events
	for (auto* callback : registered_callbacks) {
		getEventHandler()->removeEventCallback(callback);
	}

	registered_callbacks.clear();
}


void EventParticipator::setEventHandler(gsl::not_null<EventHandler*> handler) noexcept {
	event_handler = handler;
}