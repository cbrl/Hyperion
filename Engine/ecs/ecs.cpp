#include "stdafx.h"
#include "ecs.h"


ECS::ECS() {
	assert(initialized == false && "Calling ECS constructor after it has been initialized!");

	component_mgr = make_shared<ComponentMgr>();
	entity_mgr = make_unique<EntityMgr>(component_mgr);

	initialized = true;
}


ECS::~ECS() {
	initialized = false;
	entity_mgr.reset();
	component_mgr.reset();
}


// Initialize static members
bool                     ECS::initialized = false;
unique_ptr<EntityMgr>    ECS::entity_mgr;
shared_ptr<ComponentMgr> ECS::component_mgr;