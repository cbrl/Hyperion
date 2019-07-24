#include "entity_mgr.h"
#include "component/component_mgr.h"
#include "event/events.h"


namespace ecs {

EntityMgr::EntityMgr(ComponentMgr& component_mgr, EventMgr& event_mgr)
	: component_mgr(component_mgr)
	, event_mgr(event_mgr) {
}


handle64 EntityMgr::create() {
	auto [handle, resource] = entity_map.create();
	resource.get() = handle; //copy the handle into the ResourceMap's handle
	event_mgr.get().send<EntityCreated>(handle);
	return handle;
}


void EntityMgr::destroyEntity(handle64 handle) {
	if (valid(handle)) {
		event_mgr.get().send<EntityDestroyed>(handle);
		expired_entities.push_back(handle);
		component_mgr.get().removeAll(handle);
	}
}


void EntityMgr::removeExpiredEntities() {
	for (const handle64 handle : expired_entities) {
		entity_map.release(handle);
	}
	expired_entities.clear();
}


size_t EntityMgr::count() const noexcept {
	return entity_map.size();
}


bool EntityMgr::valid(handle64 entity) const noexcept {
	return entity_map.contains(entity);
}


void EntityMgr::forEach(const std::function<void(handle64)>& act) const {
	for (auto entity : entity_map) {
		act(entity);
	}
}

} // namespace ecs