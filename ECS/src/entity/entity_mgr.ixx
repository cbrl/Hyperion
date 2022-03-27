module;

#include <functional>
#include <vector>

#include "memory/handle/handle.h"
#include "memory/handle/handle_map.h"

export module ecs:entity_mgr;

import :component;
import :event_mgr;
import :events;


export namespace ecs {

//----------------------------------------------------------------------------------
// Entity Manager
//----------------------------------------------------------------------------------
//
// Handles the creation and destruction of entities
//
//----------------------------------------------------------------------------------

class EntityMgr final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	EntityMgr(ComponentMgr& component_mgr, EventMgr& event_mgr)
		: component_mgr(component_mgr)
		, event_mgr(event_mgr) {
	}

	EntityMgr(const EntityMgr& manager) = delete;
	EntityMgr(EntityMgr&& manager) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~EntityMgr() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	EntityMgr& operator=(const EntityMgr& manager) = delete;
	EntityMgr& operator=(EntityMgr&& manager) = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Entities
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	handle64 create() {
		auto [handle, resource] = entity_map.create();
		resource.get() = handle; //copy the handle into the ResourceMap's handle
		event_mgr.get().send<EntityCreated>(handle);
		return handle;
	}

	// Add an entity to the list of expired entities. Will be
	// destroyed at the end of the next ECS update.
	void destroyEntity(handle64 handle) {
		if (valid(handle)) {
			event_mgr.get().send<EntityDestroyed>(handle);
			expired_entities.push_back(handle);
			//component_mgr.get().removeAll(handle);
		}
	}

	// Remove all the entities marked for deletion. Should be called once per tick.
	void removeExpiredEntities() {
		for (const handle64 handle : expired_entities) {
			entity_map.release(handle);
		}
		expired_entities.clear();
	}

	// Get the number of entities
	[[nodiscard]]
	size_t count() const noexcept {
		return entity_map.size();
	}

	// Check if a handle is valid
	[[nodiscard]]
	bool valid(handle64 entity) const noexcept {
		return entity_map.contains(entity);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Iteration
	//----------------------------------------------------------------------------------

	// Apply an action to each entity
	void forEach(const std::function<void(handle64)>& act) const {
		for (auto entity : entity_map) {
			act(entity);
		}
	}

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// A reference to the component and event managers
	std::reference_wrapper<ComponentMgr> component_mgr;
	std::reference_wrapper<EventMgr> event_mgr;

	// Handle map. Stores valid handles in a ResourceMap, allowing for quick iteration over valid entities.
	HandleMap<handle64, handle64> entity_map;

	// A container of entities that need to be deleted
	std::vector<handle64> expired_entities;
};

} // namespace ecs
