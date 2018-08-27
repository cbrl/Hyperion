#include "entity.h"
#include "component/component_mgr.h"
#include "ecs.h"


IEntity::IEntity(EntityPtr this_ptr, ComponentMgr* component_mgr)
	: active(true)
	, this_ptr(this_ptr)
	, component_mgr(component_mgr) {
}


IEntity::~IEntity() {
	for (auto& pair : components) {
		component_mgr->destroyComponent(pair.second);
		pair.second = nullptr;
	}
}


void IEntity::setHandle(EntityPtr entity_ptr) {
	this_ptr = entity_ptr;
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


EntityPtr IEntity::getHandle() const {
	return this_ptr;
}
