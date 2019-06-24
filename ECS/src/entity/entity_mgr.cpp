#include "entity_mgr.h"
#include "component/component_mgr.h"


namespace ecs {

EntityMgr::EntityMgr(std::shared_ptr<ComponentMgr> component_mgr, EventMgr& event_mgr)
	: component_mgr(std::move(component_mgr))
	, event_mgr(event_mgr) {
}


EntityPtr EntityMgr::createEntity() {
	auto [handle, entity] = entity_map.create();

	// Create a EntityPtr
	const EntityPtr ptr = EntityPtr{this, handle};

	// Setup the entity
	entity.get().setEventMgr(gsl::make_not_null(&event_mgr));
	entity.get().setComponentMgr(gsl::make_not_null(component_mgr.get()));
	entity.get().setPointer(ptr);

	return ptr;
}


void EntityMgr::destroyEntity(handle64 handle) {
	if (valid(handle)) {
		expired_entities.push_back(handle);
		component_mgr->destroyAll(handle);

		auto& entity = *tryGetEntity(handle);
		entity.removeAllChildren();
		entity.removeParent();
	}
}


void EntityMgr::removeExpiredEntities() {
	for (const handle64 handle : expired_entities) {
		entity_map.release(handle);
	}
	expired_entities.clear();
}


Entity& EntityMgr::getEntity(handle64 handle) {
	return entity_map.get(handle);
}


Entity* EntityMgr::tryGetEntity(handle64 handle) {
	if (entity_map.contains(handle)) {
		return &entity_map.get(handle);
	}
	return nullptr;
}


size_t EntityMgr::count() const noexcept {
	return entity_map.size();
}


bool EntityMgr::valid(handle64 entity) const noexcept {
	return entity_map.contains(entity);
}


void EntityMgr::forEach(const std::function<void(Entity&)>& act) {
	for (Entity& entity : entity_map) {
		act(entity);
	}
}


void EntityMgr::forEach(const std::function<void(const Entity&)>& act) const {
	for (const Entity& entity : entity_map) {
		act(entity);
	}
}

} // namespace ecs