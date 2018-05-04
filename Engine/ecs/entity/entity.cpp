#include "stdafx.h"
#include "entity.h"
#include "ecs\ecs.h"


IEntity::IEntity()
	: active(true)
	, handle(Handle64::invalid_handle) {
}


IEntity::~IEntity() {
	for (auto& pair : components) {
		ECS::Get()->GetComponentMgr()->DestroyComponent(pair.second);
		pair.second = nullptr;
	}
}


void IEntity::SetActive(bool state) {
	active = state;
}


bool IEntity::IsActive() const {
	return active;
}


const Handle64 IEntity::GetHandle() const {
	return handle;
}
