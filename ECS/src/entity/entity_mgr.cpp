#include "entity_mgr.h"


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
		handle_table.releaseHandle(handle);
		entity_pool.deallocate(getEntity(handle));
	}

	expired_entities.clear();
	num_expired_entities = 0;
}


Entity* EntityMgr::getEntity(handle64 handle) {
	return handle_table[handle];
}


size_t EntityMgr::count() const noexcept {
	return entity_pool.getCount();
}


bool EntityMgr::validHandle(handle64 entity) const noexcept {
	return handle_table.validHandle(entity);
}
