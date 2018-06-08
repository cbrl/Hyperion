#include "entity.h"
#include "ecs.h"


IEntity::IEntity(Handle64 this_handle, ComponentMgr* component_mgr)
	: active(true)
	, handle(this_handle)
	, component_mgr(component_mgr) {
}


IEntity::~IEntity() {
	for (auto& pair : components) {
		component_mgr->DestroyComponent(pair.second);
		pair.second = nullptr;
	}
}


void IEntity::SetHandle(Handle64 this_handle) {
	handle = this_handle;
}


void IEntity::SetComponentMgr(ComponentMgr* mgr) {
	component_mgr = mgr;
}


void IEntity::SetActive(bool state) {
	active = state;
	for (auto& pair : components) {
		pair.second->SetActive(state);
	}
}


bool IEntity::IsActive() const {
	return active;
}


Handle64 IEntity::GetHandle() const {
	return handle;
}
