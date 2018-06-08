#include "entity.h"
#include "ecs.h"


IEntity::IEntity(handle64 this_handle, ComponentMgr* component_mgr)
	: active(true)
	, handle(this_handle)
	, component_mgr(component_mgr) {
}


IEntity::~IEntity() {
	for (auto& pair : components) {
		component_mgr->destroyComponent(pair.second);
		pair.second = nullptr;
	}
}


void IEntity::setHandle(handle64 this_handle) {
	handle = this_handle;
}


void IEntity::setComponentMgr(ComponentMgr* mgr) {
	component_mgr = mgr;
}


void IEntity::setActive(bool state) {
	active = state;
	for (auto& pair : components) {
		pair.second->setActive(state);
	}
}


bool IEntity::isActive() const {
	return active;
}


handle64 IEntity::getHandle() const {
	return handle;
}
