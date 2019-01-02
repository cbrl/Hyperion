#include "ecs.h"


ECS::ECS() {
	event_mgr     = std::make_unique<EventMgr>();
	system_mgr    = std::make_unique<SystemMgr>(*this, *event_mgr);
	component_mgr = std::make_shared<ComponentMgr>(*event_mgr);
	entity_mgr    = std::make_unique<EntityMgr>(component_mgr, *event_mgr);
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


void ECS::removeSystem(ISystem* system) {
	system_mgr->removeSystem(system);
}


void ECS::update(Engine& engine) {
	system_mgr->update(engine);
	event_mgr->dispatchEvents();
	entity_mgr->removeExpiredEntities();
}
