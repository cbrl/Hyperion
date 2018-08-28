#include "entity_ptr.h"
#include "entity/entity_mgr.h"


//----------------------------------------------------------------------------------
// EntityPtr
//----------------------------------------------------------------------------------

EntityPtr::EntityPtr() noexcept
	: mgr(nullptr)
	, handle(handle64::invalid_handle) {
}

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

UniqueEntityPtr::UniqueEntityPtr(EntityMgr* mgr, handle64 entity) noexcept
	: ptr(mgr, entity) {
}

UniqueEntityPtr::UniqueEntityPtr(const EntityPtr& entity) noexcept
	: ptr(entity) {
}

UniqueEntityPtr::~UniqueEntityPtr() {
	reset();
}

UniqueEntityPtr& UniqueEntityPtr::operator=(UniqueEntityPtr&& entity_ptr) noexcept {
	if (ptr.valid()) reset();
	this->ptr = std::move(entity_ptr.ptr);
	return *this;
}

IEntity* UniqueEntityPtr::operator->() const {
	return ptr.operator->();
}

IEntity& UniqueEntityPtr::operator*() const {
	return ptr.operator*();
}

IEntity* UniqueEntityPtr::get() const {
	return ptr.get();
}

EntityPtr UniqueEntityPtr::getPtr() const noexcept {
	return ptr;
}

handle64 UniqueEntityPtr::getHandle() const noexcept {
	return ptr.getHandle();
}

void UniqueEntityPtr::reset() {
	if (ptr.valid()) ptr.mgr->destroyEntity(ptr.handle);
	ptr = EntityPtr{nullptr, handle64::invalid_handle};
}
*/