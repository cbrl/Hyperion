#include "system_mgr.h"


SystemMgr::SystemMgr(ECS& ecs, EventMgr& mgr)
    : ecs(ecs)
	, event_mgr(mgr) {
}


void SystemMgr::removeSystem(ISystem* system) {
	systems.erase(system->getTypeIndex());
}


void SystemMgr::update(Engine& engine) {

	// Pre Update
	for (const auto& pair : systems) {
		if (pair.second->isActive()) {
			pair.second->preUpdate(engine);
		}
	}

	// Update
	for (const auto& pair : systems) {
		if (pair.second->isActive()) {
			pair.second->update(engine);
		}
	}

	// Post Update
	for (const auto& pair : systems) {
		if (pair.second->isActive()) {
			pair.second->postUpdate(engine);
		}
	}
}
