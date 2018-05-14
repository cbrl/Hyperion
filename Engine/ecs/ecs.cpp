#include "stdafx.h"
#include "ecs.h"


ECS* ECS::engine = nullptr;


ECS::ECS() {
	if (engine != nullptr) return;

	component_mgr = make_unique<ComponentMgr>();
	entity_mgr    = make_unique<EntityMgr>(component_mgr);
	system_mgr    = make_unique<SystemMgr>();
	engine        = this;
}


ECS::~ECS() {
	entity_mgr.reset();
	component_mgr.reset();

	engine = nullptr;
}
