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


handle64 ECS::create() {
	return entity_mgr->create();
}


void ECS::destroy(handle64 entity) {
	entity_mgr->destroyEntity(entity);
}


bool ECS::valid(handle64 handle) const {
	return entity_mgr->valid(handle);
}


void ECS::remove(handle64 entity, IComponent& component) {
	component_mgr->remove(entity, component);
}


void ECS::remove(ISystem& system) {
	system_mgr->remove(system);
}


void ECS::update(std::chrono::duration<f64> dt) {
	system_mgr->update(dt);
	event_mgr->dispatch();
	entity_mgr->removeExpiredEntities();
	component_mgr->removeExpiredComponents();
}

} // namespace ecs