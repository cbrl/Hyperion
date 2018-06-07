#pragma once

#include "entity\entity.h"
#include "component\component_mgr.h"
#include "handle\handle_table.h"



//----------------------------------------------------------------------------------
// Entity Manager
//----------------------------------------------------------------------------------
//
// Handles the creation and destruction of entities
//
//----------------------------------------------------------------------------------

class EntityMgr final {
	public:
		EntityMgr(shared_ptr<ComponentMgr> component_mgr)
			: num_expired_entities(0)
			, component_mgr(component_mgr)
		{}

		~EntityMgr() = default;


		template<typename EntityT, typename... ArgsT>
		Handle64 CreateEntity(ArgsT&&... args) {

			static_assert(std::is_base_of_v<IEntity, EntityT>,
						  "Calling EntityMgr::CreateEntity() with non-entity type.");

			// Get the proper entity pool
			auto pool = entity_pools.GetOrCreatePool<EntityT>();

			// Allocate memory for the entity
			void* memory = pool->AllocateObject();

			// Create a handle
			Handle64 handle = handle_table.CreateHandle(static_cast<IEntity*>(memory));

			// Create the entity
			EntityT* entity = new(memory) EntityT(handle, component_mgr.get(), std::forward<ArgsT>(args)...);

			return handle;
		}


		// Add an entity to the list of expired entities. Will be
		// destroyed at the end of the next ECS update.
		void DestroyEntity(Handle64 handle) {

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
		void RemoveExpiredEntities() {
			for (Handle64 handle : expired_entities) {

				// Get the entity
				IEntity* entity = handle_table[handle];

				// Get the entity type_index
				auto type = handle_table[handle]->GetTypeID();

				// Get the appropriate pool with the type_index
				auto pool = entity_pools.GetPool(type);

				// Destroy the entity
				handle_table.ReleaseHandle(handle);
				pool->DestroyObject(static_cast<void*>(entity));
			}

			num_expired_entities = 0;
		}


		IEntity* GetEntity(Handle64 handle) {
			return handle_table[handle];
		}


		template<typename EntityT>
		size_t CountOf() {
			return entity_pools.PoolExists<EntityT>() ? entity_pools.GetPool<EntityT>()->Count() : 0;
		}


		template<typename EntityT>
		bool KnowsEntity() const {
			return entity_pools.PoolExists<EntityT>();
		}


		template<typename EntityT>
		typename ResourcePool<EntityT>::iterator begin() {
			using pool_t = ResourcePool<EntityT>;
			auto* pool = static_cast<pool_t*>(entity_pools.GetPool<EntityT>());
			return pool->begin();
		}

		template<typename EntityT>
		typename ResourcePool<EntityT>::iterator end() {
			using pool_t = ResourcePool<EntityT>;
			auto* pool = static_cast<pool_t*>(entity_pools.GetPool<EntityT>());
			return pool->end();
		}


	private:
		// A pointer to the component manager
		shared_ptr<ComponentMgr> component_mgr;

		// Map of unique resource pools for each type of entity
		ResourcePoolFactory entity_pools;

		// Handle table, which maps handles to a pointer to an entity
		HandleTable<Handle64, IEntity> handle_table;

		// Container of entities that need to be deleted
		vector<Handle64> expired_entities;
		u32              num_expired_entities;
};