#pragma once

#include "ecs\entity\entity.h"
#include "ecs\component\component_mgr.h"
#include "ecs\handle\handle_table.h"



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
			: component_mgr(component_mgr)
		{}

		~EntityMgr() = default;


		template<typename EntityT, typename... ArgsT>
		Handle64 CreateEntity(ArgsT&&... args) {

			static_assert(std::is_base_of_v<IEntity, EntityT>,
						  "Calling EntityMgr::CreateEntity() with non-entity type.");

			// Get the proper entity pool
			auto pool = entity_pools.GetOrCreatePool<EntityT>();

			// Allocate memory for the entity
			void* memory = pool->CreateObject();

			// Create a handle
			Handle64 handle = handle_table.CreateHandle(static_cast<IEntity*>(memory));

			// Create the entity
			EntityT* entity = new(memory) EntityT(std::forward<ArgsT>(args)...);
			entity->handle = handle;

			return handle;
		}


		void DestroyEntity(Handle64 handle) {

			// Get the entity
			IEntity* entity = handle_table[handle];

			// Get the entity type_index
			auto type = handle_table[handle]->GetTypeID();

			// Get the appropriate pool witht the type_index
			auto pool = entity_pools.GetPool(type);

			pool->DestroyObject(static_cast<void*>(entity));

			handle_table.ReleaseHandle(handle);
		}


		IEntity* GetEntity(Handle64 handle) {
			return handle_table[handle];
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
		// Map of unique resource pools for each type of entity
		ResourcePoolMap entity_pools;

		// Handle table, which maps handles to a pointer to an entity
		HandleTable<Handle64, IEntity> handle_table;

		// A pointer to the component manager
		shared_ptr<ComponentMgr> component_mgr;
};