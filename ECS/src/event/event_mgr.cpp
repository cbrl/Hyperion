#include "event_mgr.h"


namespace ecs {

void EventMgr::clearDispatchers() {
	event_dispatchers.clear();
}

void EventMgr::dispatch() {
	for (auto& [id, dispatcher] : event_dispatchers) {
		dispatcher->dispatch();
	}
}

} // namespace ecs