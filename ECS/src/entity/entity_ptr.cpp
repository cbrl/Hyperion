#include "entity_ptr.h"
#include "entity/entity_mgr.h"


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

handle64 EntityPtr::getHandle() const {
	return handle;
}

bool EntityPtr::valid() const {
	if (!mgr) return false;
	return mgr->validHandle(handle);
}