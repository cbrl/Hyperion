#pragma once

#include "ecs\entity.h"
#include "ecs\component_mgr.h"
#include "ecs\handle\handle_table.h"


//----------------------------------------------------------------------------------
// EntityPool
//----------------------------------------------------------------------------------

class IEntityPool {
	public:
		virtual ~IEntityPool() = default;

		virtual void DestroyEntity(void* entity) = 0;
};

template<typename EntityT, size_t chunk_objects = 512>
class EntityPool : public IEntityPool, public ResourcePool<EntityT, chunk_objects> {
	public:
		~EntityPool() = default;

		void DestroyEntity(void* entity) {
			this->DestroyObject(entity);
		}
};


//----------------------------------------------------------------------------------
// EntityPool Map
//----------------------------------------------------------------------------------

class EntityPoolMap final {
	public:
		EntityPoolMap() = default;

		~EntityPoolMap() {
			for (auto& pair : pools) {
				delete pair.second;
				pair.second = nullptr;
			}
		}


		template<typename EntityT>
		EntityPool<EntityT>* GetOrCreatePool() {

			using pool_t = EntityPool<EntityT>;

			const auto it = pools.find(EntityT::type_idx);

			if (it == pools.end()) {
				pools[EntityT::type_idx] = new pool_t();
				return static_cast<pool_t*>(pools[EntityT::type_idx]);
			}

			return static_cast<pool_t*>(it->second);
		}


		template<typename EntityT>
		EntityPool<EntityT>* GetPool() {
			const auto it = pools.find(EntityT::type_idx);

			assert(it != pools.end() && "Invalid entity pool type requested");

			return it->second;
		}


		IEntityPool* GetPool(type_index type) {
			const auto it = pools.find(type);

			assert(it != pools.end() && "Invalid entity pool type requested");

			return it->second;
		}


	private:
		unordered_map<type_index, IEntityPool*> pools;
};



//----------------------------------------------------------------------------------
// Entity Manager
//----------------------------------------------------------------------------------

class EntityMgr final {
	public:
		EntityMgr(shared_ptr<ComponentMgr> component_mgr)
			: component_mgr(component_mgr)
		{}

		~EntityMgr() = default;


		template<typename EntityT, typename... ArgsT>
		Handle64 CreateEntity(ArgsT&&... args) {

			// Get the proper entity pool
			auto pool = entity_pools.GetOrCreatePool<EntityT>();

			// Allocate memory for the entity
			void* memory = pool->CreateObject();

			// Create a handle
			Handle64 handle = handle_table.CreateHandle(static_cast<IEntity*>(memory));

			// Create the entity
			EntityT* entity = new(memory) EntityT(std::forward(args)...);
			entity->component_mgr = component_mgr;

			return handle;
		}


		void DestroyEntity(Handle64 handle) {

			// Get the entity
			IEntity* entity = handle_table[handle];

			// Get the entity type_index
			auto type = handle_table[handle]->GetTypeIDX();

			// Get the appropriate pool witht the type_index
			auto pool = entity_pools.GetPool(type);

			pool->DestroyEntity(static_cast<void*>(entity));
		}


		IEntity* GetEntity(Handle64 handle) {
			return handle_table[handle];
		}


	private:
		EntityPoolMap entity_pools;
		HandleTable<Handle64, IEntity> handle_table;

		shared_ptr<ComponentMgr> component_mgr;
};