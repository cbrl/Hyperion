#pragma once

#include "entity/entity.h"
#include "component/component_mgr.h"
#include "memory/handle/handle_table.h"


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

	EntityMgr(shared_ptr<ComponentMgr> component_mgr)
		: component_mgr(std::move(component_mgr))
		, num_expired_entities(0) {
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
	shared_ptr<ComponentMgr> component_mgr;

	// Map of unique resource pools for each type of entity
	ResourcePool<Entity> entity_pool;

	// Handle table. Maps a handle to an Entity pointer.
	HandleTable<handle64, Entity> handle_table;

	// Container of entities that need to be deleted
	std::vector<handle64> expired_entities;
	u32 num_expired_entities;
};
