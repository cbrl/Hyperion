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

		IEntity* entity = getEntity(handle);
		const auto type = entity->getTypeIndex();

		auto* pool = entity_pools.getPool(type);

		// Destroy the entity
		handle_table.releaseHandle(handle);
		pool->deallocate(entity);
	}

	expired_entities.clear();
	num_expired_entities = 0;
}
