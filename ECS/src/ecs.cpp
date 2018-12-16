#include "ecs.h"


ECS::ECS() {
	component_mgr = std::make_unique<ComponentMgr>();
	entity_mgr    = std::make_unique<EntityMgr>(component_mgr);
	system_mgr    = std::make_unique<SystemMgr>();
}


ECS::~ECS() {
	entity_mgr.reset();
	component_mgr.reset();
}


EntityPtr ECS::createEntity() {
	return entity_mgr->createEntity();
}


void ECS::destroyEntity(handle64 entity) {
	entity_mgr->destroyEntity(entity);
}


void ECS::update(Engine& engine) {
	system_mgr->update(engine);
	entity_mgr->removeExpiredEntities();
}
