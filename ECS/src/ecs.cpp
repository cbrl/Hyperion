#include "ecs.h"
#include "event/event_mgr.h"
#include "system/system_mgr.h"
#include "component/component_mgr.h"
#include "entity/entity_mgr.h"

namespace ecs {

ECS::ECS() {
	event_mgr     = std::make_unique<EventMgr>();
	system_mgr    = std::make_unique<SystemMgr>(*this);
	component_mgr = std::make_unique<ComponentMgr>(*event_mgr);
	entity_mgr    = std::make_unique<EntityMgr>(*component_mgr, *event_mgr);
}


handle64 ECS::createEntity() {
	return entity_mgr->createEntity();
}


void ECS::removeEntity(handle64 entity) {
	entity_mgr->destroyEntity(entity);
}


Entity& ECS::get(handle64 handle) {
	return entity_mgr->get(handle);
}


const Entity& ECS::get(handle64 handle) const {
	return entity_mgr->get(handle);
}


Entity* ECS::tryGet(handle64 handle) {
	return entity_mgr->tryGet(handle);
}


const Entity* ECS::tryGet(handle64 handle) const {
	return entity_mgr->tryGet(handle);
}


bool ECS::valid(handle64 handle) const {
	return entity_mgr->valid(handle);
}


void ECS::removeComponent(handle64 entity, IComponent& component) {
	component_mgr->removeComponent(entity, component);
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