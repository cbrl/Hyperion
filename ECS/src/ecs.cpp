#include "ecs.h"


ECS::ECS() {
	component_mgr = std::make_unique<ComponentMgr>();
	entity_mgr = std::make_unique<EntityMgr>(component_mgr);
	system_mgr = std::make_unique<SystemMgr>();
}


ECS::~ECS() {
	entity_mgr.reset();
	component_mgr.reset();
}


void ECS::update(const Engine& engine) {
	system_mgr->update(engine);
	entity_mgr->removeExpiredEntities();
}


void ECS::destroyEntity(handle64 handle) {
	entity_mgr->destroyEntity(handle);
}


IEntity* ECS::getEntity(handle64 handle) {
	return entity_mgr->getEntity(handle);
}
