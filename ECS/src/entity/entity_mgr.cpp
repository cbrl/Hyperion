#include "entity_mgr.h"


EntityPtr EntityMgr::createEntity() {

	auto& entity = entity_pool.emplace_back();

	// Create a handle
	const handle64 handle = handle_map.createHandle(&entity);
	const EntityPtr ptr = EntityPtr{this, handle};

	// Create the entity
	entity.setComponentMgr(gsl::make_not_null(component_mgr.get()));
	entity.setPointer(ptr);

	return ptr;
}


void EntityMgr::destroyEntity(handle64 handle) {

	if (expired_entities.size() > num_expired_entities) {
		expired_entities[num_expired_entities] = handle;
		++num_expired_entities;
	}
	else {
		expired_entities.push_back(handle);
		++num_expired_entities;
	}
}


void EntityMgr::removeExpiredEntities() {

	for (const handle64 handle : expired_entities) {
		// Destroy the entity
		entity_pool.remove_resource(getEntity(handle));
		handle_map.releaseHandle(handle);
	}

	expired_entities.clear();
	num_expired_entities = 0;
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
