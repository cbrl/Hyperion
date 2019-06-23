#include "ecs.h"


namespace ecs {

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
	component_mgr->destroyAll(entity);
}


void ECS::removeSystem(ISystem& system) {
	system_mgr->removeSystem(system);
}


void ECS::update(std::chrono::duration<f64> dt) {
	system_mgr->update(dt);
	event_mgr->dispatchEvents();
	entity_mgr->removeExpiredEntities();
	component_mgr->removeExpiredComponents();
}

} // namespace ecs