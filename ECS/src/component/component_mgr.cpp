#include "component_mgr.h"


namespace ecs {

ComponentMgr::ComponentMgr(EventMgr& event_mgr)
	: event_mgr(event_mgr) {
}


void ComponentMgr::remove(handle64 entity, IComponent& component) {
	const auto it = component_pools.find(component.getTypeIndex());
	if (it != component_pools.end()) {
		if (it->second->contains(entity.index)) {
			expired_components[component.getTypeIndex()].push_back(entity);
		}
		else {
			Logger::log(LogLevel::err, "Attempting to remove a component from an entity that does not contain it");
			assert(false);
		}
	}
}


void ComponentMgr::removeAll(handle64 entity) {
	for (const auto& [index, pool] : component_pools) {
		if (pool->contains(entity.index)) {
			expired_components[index].push_back(entity);
		}
	}
}


void ComponentMgr::removeExpiredComponents() {
	for (auto& [type, vec] : expired_components) { //for each vector of a component type
		const auto it = component_pools.find(type);

		if (it != component_pools.end()) { //if the associated pool exists
			for (auto& handle : vec) { //destroy each entity's component
				it->second->erase(handle.index);
			}				
		}
		else {
			Logger::log(
				LogLevel::err,
				"Could not find appropriate pool when destroying a component of type \"{}\"",
				type.name()
			);
			assert(false);
		}

		// Clear the vector after all components have been processed
		vec.clear();
	}
}

} //namespace ecs