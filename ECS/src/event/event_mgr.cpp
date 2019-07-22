#include "event_mgr.h"


namespace ecs {

void EventMgr::clearEventDispatchers() {
	event_dispatchers.clear();
}

void EventMgr::dispatchEvents() {
	for (auto& [id, dispatcher] : event_dispatchers) {
		dispatcher->dispatch();
	}
}

} // namespace ecs