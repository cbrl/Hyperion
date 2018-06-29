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
	explicit EntityMgr(shared_ptr<ComponentMgr> component_mgr)
		: component_mgr(std::move(component_mgr))
		, num_expired_entities(0) {
	}

	~EntityMgr() = default;


	template<typename EntityT, typename... ArgsT>
	handle64 createEntity(ArgsT&&... args) {

		static_assert(std::is_base_of_v<IEntity, EntityT>,
			"Calling EntityMgr::CreateEntity() with non-entity type.");

		// Get the proper entity pool
		auto pool = entity_pools.getOrCreatePool<EntityT>();

		// Allocate memory for the entity
		void* memory = pool->allocateObject();

		// Create a handle
		handle64 handle = handle_table.createHandle(static_cast<IEntity*>(memory));

		// Create the entity
		EntityT* entity = new(memory) EntityT(handle, component_mgr.get(), std::forward<ArgsT>(args)...);

		return handle;
	}


	// Add an entity to the list of expired entities. Will be
	// destroyed at the end of the next ECS update.
	void destroyEntity(handle64 handle) {

		if (expired_entities.size() > num_expired_entities) {
			expired_entities[num_expired_entities] = handle;
			++num_expired_entities;
		}
		else {
			expired_entities.push_back(handle);
			++num_expired_entities;
		}
	}


	// Remove all the entities marked for deletion. Should be
	// called once per tick.
	void removeExpiredEntities() {
		for (const handle64 handle : expired_entities) {

			// Get the entity
			IEntity* entity = handle_table[handle];

			// Get the entity type_index
			const auto type = handle_table[handle]->getTypeId();

			// Get the appropriate pool with the type_index
			auto pool = entity_pools.getPool(type);

			// Destroy the entity
			handle_table.releaseHandle(handle);
			pool->destroyObject(entity);
		}

		expired_entities.clear();
		num_expired_entities = 0;
	}


	IEntity* getEntity(handle64 handle) {
		return handle_table[handle];
	}


	template<typename EntityT>
	size_t countOf() {
		return entity_pools.poolExists<EntityT>() ? entity_pools.getPool<EntityT>()->Count() : 0;
	}


	// Check if the entity manager has created an entity of the specified type
	template<typename EntityT>
	bool knowsEntity() const {
		return entity_pools.poolExists<EntityT>();
	}


	// Apply an action to each entity
	template<typename EntityT, typename ActionT>
	void forEach(ActionT act) {
		using pool_t = ResourcePool<EntityT>;
		auto* pool = static_cast<pool_t*>(entity_pools.getPool<EntityT>());
		pool->forEach(act);
	}


private:
	// A pointer to the component manager
	shared_ptr<ComponentMgr> component_mgr;

	// Map of unique resource pools for each type of entity
	ResourcePoolFactory entity_pools;

	// Handle table, which maps handles to a pointer to an entity
	HandleTable<handle64, IEntity> handle_table;

	// Container of entities that need to be deleted
	vector<handle64> expired_entities;
	u32 num_expired_entities;
};
