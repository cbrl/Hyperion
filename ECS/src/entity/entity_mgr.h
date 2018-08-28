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

	explicit EntityMgr(shared_ptr<ComponentMgr> component_mgr)
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

	template<typename EntityT, typename... ArgsT>
	[[nodiscard]]
	EntityPtr createEntity(ArgsT&&... args);


	// Add an entity to the list of expired entities. Will be
	// destroyed at the end of the next ECS update.
	void destroyEntity(handle64 handle);


	// Remove all the entities marked for deletion. Should be called once per tick.
	void removeExpiredEntities();


	// Get an entity from the handle
	[[nodiscard]]
	IEntity* getEntity(handle64 handle) {
		return handle_table[handle];
	}


	// Get the number of the specified entity type
	template<typename EntityT>
	[[nodiscard]]
	size_t countOf() {
		return entity_pools.poolExists<EntityT>() ? entity_pools.getPool<EntityT>()->getCount() : 0;
	}


	// Check if the entity manager has created an entity of the specified type
	template<typename EntityT>
	[[nodiscard]]
	bool knowsEntity() const {
		return entity_pools.poolExists<EntityT>();
	}


	[[nodiscard]]
	bool validHandle(handle64 entity) const {
		return handle_table.validHandle(entity);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Iteration
	//----------------------------------------------------------------------------------

	// Apply an action to each entity
	template<typename EntityT, typename ActionT>
	void forEach(ActionT&& act);


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// A pointer to the component manager
	shared_ptr<ComponentMgr> component_mgr;

	// Map of unique resource pools for each type of entity
	ResourcePoolFactory entity_pools;

	// Handle table, which maps handles to a pointer to an entity
	HandleTable<handle64, IEntity> handle_table;

	// Container of entities that need to be deleted
	std::vector<handle64> expired_entities;
	u32 num_expired_entities;
};


#include "entity_mgr.tpp"