#include "entity_mgr.h"


EntityPtr EntityMgr::createEntity() {

	auto& entity = entity_pool.emplace_back();
	entity.setEventMgr(gsl::make_not_null(&event_mgr));

	// Create a handle
	const handle64  handle = handle_map.createHandle(&entity);
	const EntityPtr ptr    = EntityPtr{this, handle};

	// Create the entity
	entity.setComponentMgr(gsl::make_not_null(component_mgr.get()));
	entity.setPointer(ptr);

	return ptr;
}


void EntityMgr::destroyEntity(handle64 handle) {
	expired_entities.push_back(handle);
}


void EntityMgr::removeExpiredEntities() {

	for (const handle64 handle : expired_entities) {
		// Destroy the entity
		entity_pool.remove_resource(getEntity(handle));
		handle_map.releaseHandle(handle);
	}

	expired_entities.clear();
}


Entity* EntityMgr::getEntity(handle64 handle) {
	return handle_map[handle];
}


size_t EntityMgr::count() const noexcept {
	return entity_pool.size();
}


bool EntityMgr::validHandle(handle64 entity) const noexcept {
	return handle_map.validHandle(entity);
}
