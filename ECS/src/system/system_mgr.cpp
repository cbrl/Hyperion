#include "system_mgr.h"


SystemMgr::SystemMgr(ECS& ecs, EventMgr& mgr)
    : ecs(ecs)
	, event_mgr(mgr) {
}


void SystemMgr::removeSystem(ISystem& system) {
	systems.erase(system.getTypeIndex());
}


void SystemMgr::sortSystemQueue() {
	std::sort(system_queue.begin(), system_queue.end(), PriorityCompareGreater());
}


void SystemMgr::update(std::chrono::duration<f64> dt) {

	//----------------------------------------------------------------------------------
	// Pre Update
	//----------------------------------------------------------------------------------
	for (ISystem& system : system_queue) {

		if (system.isActive()) {
			system.time_since_last_update += dt; //update system delta time

			if (system.time_since_last_update >= system.update_interval) {
				system.needs_update = true;
			}

			if (system.needs_update) {
				system.preUpdate();
			}
		}
	}

	//----------------------------------------------------------------------------------
	// Update
	//----------------------------------------------------------------------------------
	for (ISystem& system : system_queue) {
		if (system.isActive() && system.needs_update) {
			system.update();
		}
	}

	//----------------------------------------------------------------------------------
	// Post Update
	//----------------------------------------------------------------------------------
	for (ISystem& system : system_queue) {
		if (system.isActive() && system.needs_update) {
			system.postUpdate();
			system.needs_update = false;
			system.time_since_last_update = 0.0s;
		}
	}
}
