#include "entity_ptr.h"
#include "entity/entity_mgr.h"


//----------------------------------------------------------------------------------
// EntityPtr
//----------------------------------------------------------------------------------

Entity* EntityPtr::operator->() const {
	return mgr->getEntity(handle);
}

Entity& EntityPtr::operator*() const {
	return *(mgr->getEntity(handle));
}

Entity* EntityPtr::get() const {
	return mgr->getEntity(handle);
}

handle64 EntityPtr::getHandle() const noexcept {
	return handle;
}

bool EntityPtr::valid() const noexcept {
	if (!mgr) return false;
	return mgr->validHandle(handle);
}



/*
//----------------------------------------------------------------------------------
// UniqueEntityPtr
//----------------------------------------------------------------------------------

void UniqueEntityPtr::reset() {
	if (ptr.valid()) ptr.mgr->destroyEntity(ptr.handle);
	ptr = EntityPtr{nullptr, handle64::invalid_handle};
}
*/