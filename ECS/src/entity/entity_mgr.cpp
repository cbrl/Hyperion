#include "entity_mgr.h"
#include "component/component_mgr.h"


namespace ecs {

EntityMgr::EntityMgr(ComponentMgr& component_mgr, EventMgr& event_mgr)
	: component_mgr(component_mgr)
	, event_mgr(event_mgr) {
}


handle64 EntityMgr::createEntity() {
	auto [handle, entity] = entity_map.create();

	// Setup the entity
	entity.get().setHandle(handle);
	entity.get().setEventMgr(gsl::make_not_null(&event_mgr.get()));
	entity.get().setEntityMgr(gsl::make_not_null(this));
	entity.get().setComponentMgr(gsl::make_not_null(&component_mgr.get()));

	return handle;
}


void EntityMgr::destroyEntity(handle64 handle) {
	if (valid(handle)) {
		expired_entities.push_back(handle);
		component_mgr.get().removeAll(handle);

		auto& entity = *tryGet(handle);
		entity.removeAllChildren();
		entity.removeParent();
	}
}


void EntityMgr::removeExpiredEntities() {
	for (const handle64 handle : expired_entities) {
		entity_map.release(handle);
	}
	expired_entities.clear();
}


Entity& EntityMgr::get(handle64 handle) {
	return entity_map.get(handle);
}


Entity* EntityMgr::tryGet(handle64 handle) {
	if (entity_map.contains(handle)) {
		return &entity_map.get(handle);
	}
	return nullptr;
}


size_t EntityMgr::count() const noexcept {
	return entity_map.size();
}


bool EntityMgr::valid(handle64 entity) const noexcept {
	return entity_map.contains(entity);
}


void EntityMgr::forEach(const std::function<void(Entity&)>& act) {
	for (Entity& entity : entity_map) {
		act(entity);
	}
}


void EntityMgr::forEach(const std::function<void(const Entity&)>& act) const {
	for (const Entity& entity : entity_map) {
		act(entity);
	}
}

} // namespace ecs