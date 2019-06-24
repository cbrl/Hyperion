#include "entity_ptr.h"
#include "entity/entity_mgr.h"


namespace ecs {

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
	if (!valid()) return nullptr;
	return mgr->getEntity(handle);
}

handle64 EntityPtr::getHandle() const noexcept {
	return handle;
}

bool EntityPtr::valid() const noexcept {
	if (!mgr) return false;
	return mgr->valid(handle);
}



/*
//----------------------------------------------------------------------------------
// UniqueEntityPtr
//----------------------------------------------------------------------------------

UniqueEntityPtr& UniqueEntityPtr::operator=(UniqueEntityPtr&& ptr) noexcept {
	reset();
	EntityPtr new_ptr = ptr.release();
	mgr = new_ptr.mgr;
	handle = new_ptr.handle;
	return *this;
}

void UniqueEntityPtr::reset() {
	if (valid()) mgr->destroyEntity(handle);
	mgr = nullptr;
	handle = handle64::invalid_handle;
}

EntityPtr UniqueEntityPtr::release() {
	EntityPtr out{mgr, handle};
	mgr = nullptr;
	handle = handle64::invalid_handle;
	return out;
}
*/

} // namespace ecs