#include "entity_mgr.h"


//----------------------------------------------------------------------------------
// EntityPtr
//----------------------------------------------------------------------------------

EntityPtr::EntityPtr(EntityMgr* mgr, handle64 entity) noexcept
	: mgr(mgr)
	, handle(entity) {
}


IEntity* EntityPtr::operator->() const {
	return mgr->getEntity(handle);
}

IEntity& EntityPtr::operator*() const {
	return *(mgr->getEntity(handle));
}


IEntity* EntityPtr::get() const {
	return mgr->getEntity(handle);
}

void EntityPtr::reset() {
	mgr = nullptr;
	handle = handle64::invalid_handle;
}

handle64 EntityPtr::getHandle() const {
	return handle;
}

bool EntityPtr::valid() const {
	if (!mgr) return false;
	return mgr->validHandle(handle);
}




//----------------------------------------------------------------------------------
// EntityMgr
//----------------------------------------------------------------------------------

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

		auto pool = entity_pools.getPool(type);

		// Destroy the entity
		handle_table.releaseHandle(handle);
		pool->destroyObject(entity);
	}

	expired_entities.clear();
	num_expired_entities = 0;
}
