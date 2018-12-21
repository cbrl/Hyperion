#pragma once

#include "entity/entity.h"
#include "component/component_mgr.h"
#include "memory/handle/handle_map.h"


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

	EntityMgr(std::shared_ptr<ComponentMgr> component_mgr, EventMgr& event_mgr)
		: component_mgr(std::move(component_mgr))
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
	EntityPtr createEntity();

	// Add an entity to the list of expired entities. Will be
	// destroyed at the end of the next ECS update.
	void destroyEntity(handle64 handle);


	// Remove all the entities marked for deletion. Should be called once per tick.
	void removeExpiredEntities();


	// Get the entity associated with the handle
	[[nodiscard]]
	Entity* getEntity(handle64 handle);


	// Get the number of entities
	[[nodiscard]]
	size_t count() const noexcept;


	// Check if a handle is valid
	[[nodiscard]]
	bool validHandle(handle64 entity) const noexcept;


	//----------------------------------------------------------------------------------
	// Member Functions - Iteration
	//----------------------------------------------------------------------------------

	// Apply an action to each entity
	template<typename ActionT>
	void forEach(ActionT&& act) {
		for (Entity& entity : entity_pool) {
			act(entity);
		}
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// A pointer to the component manager
	std::shared_ptr<ComponentMgr> component_mgr;

	// A reference to the event manager. Passed to the entity (EventSender).
	EventMgr& event_mgr;

	// Map of unique resource pools for each type of entity
	ResourcePool<Entity> entity_pool;

	// Handle map. Maps a handle to an Entity pointer.
	HandleMap<handle64, Entity*> handle_map;

	// Container of entities that need to be deleted
	std::vector<handle64> expired_entities;
};
