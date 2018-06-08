#include "ecs.h"


ECS::ECS() {
	component_mgr = make_unique<ComponentMgr>();
	entity_mgr = make_unique<EntityMgr>(component_mgr);
	system_mgr = make_unique<SystemMgr>();
}


ECS::~ECS() {
	entity_mgr.reset();
	component_mgr.reset();
}


void ECS::update(const Engine& engine) const {
	system_mgr->update(engine);
	entity_mgr->removeExpiredEntities();
}


void ECS::destroyEntity(handle64 entity) const {
	entity_mgr->destroyEntity(entity);
}
