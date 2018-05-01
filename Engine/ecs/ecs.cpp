#include "stdafx.h"
#include "ecs.h"


ECS* ECS::Engine = nullptr;


ECS::ECS() {
	if (Engine != nullptr) return;

	component_mgr = make_shared<ComponentMgr>();
	entity_mgr    = make_unique<EntityMgr>(component_mgr);
	Engine        = this;
}


ECS::~ECS() {
	entity_mgr.reset();
	component_mgr.reset();

	Engine = nullptr;
}
