#include "event_mgr.h"


namespace ecs {

void EventMgr::clearEventBuffer() {
	//destroy events
	for (auto* event : events) {
		event->~IEvent();
	}
	events.clear();
	event_pool.release(); //clear allocated memory
}

void EventMgr::clearEventDispatchers() {
	event_dispatchers.clear();
}

void EventMgr::dispatchEvents() {
	size_t index = 0;
	size_t last_index = events.size();

	while (index < last_index) {
		auto* event = events[index++];
		if (!event) {
			Logger::log(LogLevel::err, "Skipping invalid event");
			continue;
		}

		if (auto it = event_dispatchers.find(event->getID()); it != event_dispatchers.end()) {
			it->second->dispatch(*event);

			// update last index, after dispatch operation there could be new events
			last_index = events.size();
		}
	}

	clearEventBuffer(); //reset
}

void EventMgr::removeEventCallback(gsl::not_null<IEventDelegate*> eventDelegate) {
	auto index = eventDelegate->getEventID();
	if (auto it = event_dispatchers.find(index); it != event_dispatchers.end()) {
		it->second->removeEventCallback(eventDelegate);
	}
}

} // namespace ecs