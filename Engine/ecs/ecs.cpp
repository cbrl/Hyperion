#include "stdafx.h"
#include "ecs.h"


ECS::ECS() {
	component_mgr = make_unique<ComponentMgr>();
	entity_mgr    = make_unique<EntityMgr>(component_mgr);
	system_mgr    = make_unique<SystemMgr>();
}


ECS::~ECS() {
	entity_mgr.reset();
	component_mgr.reset();
}


void ECS::Update(const Engine& engine) {
	system_mgr->Update(engine);
	entity_mgr->RemoveExpiredEntities();
}


void ECS::DestroyEntity(Handle64 entity) {
	entity_mgr->DestroyEntity(entity);
}