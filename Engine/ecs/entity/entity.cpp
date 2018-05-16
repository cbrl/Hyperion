#include "stdafx.h"
#include "entity.h"
#include "ecs\ecs.h"


IEntity::IEntity()
	: active(true) {
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


const Handle64 IEntity::GetHandle() const {
	return handle;
}