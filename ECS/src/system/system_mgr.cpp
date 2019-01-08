#include "system_mgr.h"


SystemMgr::SystemMgr(ECS& ecs, EventMgr& mgr)
    : ecs(ecs)
	, event_mgr(mgr) {
}


void SystemMgr::removeSystem(ISystem* system) {
	systems.erase(system->getTypeIndex());
}


void SystemMgr::update(Engine& engine, f32 dt) {

	// Pre Update
	for (auto& [index, system] : systems) {
		
		system->time_since_last_update += dt;

		if (system->isActive()) {
			if (system->time_since_last_update > system->update_interval) {
				system->needs_update = true;
			}
			if (system->needs_update) {
				system->preUpdate(engine);
			}
		}
	}

	// Update
	for (auto& [index, system] : systems) {
		if (system->isActive() && system->needs_update) {
			system->update(engine);
		}
	}

	// Post Update
	for (auto& [index, system] : systems) {
		if (system->isActive() && system->needs_update) {
			system->postUpdate(engine);
			system->needs_update = false;
			system->time_since_last_update = 0.0f;
		}
	}
}
