#pragma once

#include "util\memory\resource_pool.h"
#include "ecs\component.h"


//----------------------------------------------------------------------------------
// ComponentPool
//----------------------------------------------------------------------------------
//
// A wrapper around a resource pool
//
//----------------------------------------------------------------------------------

class IComponentPool {
	public:
		virtual ~IComponentPool() = default;

		virtual void DestroyComponent(void* component) = 0;
};

template<typename ComponentT, size_t chunk_objects = 512>
class ComponentPool final : public IComponentPool, public ResourcePool<ComponentT, chunk_objects> {
	public:
		~ComponentPool() = default;

		void DestroyComponent(void* component) {
			this->DestroyObject(component);
		}
};



//----------------------------------------------------------------------------------
// ComponentPool Map
//----------------------------------------------------------------------------------
//
// Creates a unique resource pool for each type of component
//
//----------------------------------------------------------------------------------

class ComponentPoolMap final {
	public:
		ComponentPoolMap() = default;

		~ComponentPoolMap() {
			for (auto& pair : pools) {
				delete pair.second;
				pair.second = nullptr;
			}
		}


		template<typename ComponentT>
		ComponentPool<ComponentT>* GetOrCreatePool() {

			using pool_t = ComponentPool<ComponentT>;

			const auto it = pools.find(ComponentT::type_idx);

			if (it == pools.end()) {
				pools[ComponentT::type_idx] = new pool_t();
				return static_cast<pool_t*>(pools[ComponentT::type_idx]);
			}

			return static_cast<pool_t*>(it->second);
		}


		template<typename ComponentT>
		ComponentPool<ComponentT>* GetPool() {
			const auto it = pools.find(ComponentT::type_idx);

			assert(it != pools.end() && "Invalid component pool type requested");

			return static_cast<ComponentPool<ComponentT>>(it->second);
		}


		IComponentPool* GetPool(type_index type) {
			const auto it = pools.find(type);

			assert(it != pools.end() && "Invalid component pool type requested");

			return it->second;
		}


	private:
		unordered_map<type_index, IComponentPool*> pools;
};



//----------------------------------------------------------------------------------
// Component Manager
//----------------------------------------------------------------------------------
//
// Handles the creation and destruction of components
//
//----------------------------------------------------------------------------------

class ComponentMgr final {
	public:
		ComponentMgr() = default;
		~ComponentMgr() = default;


		template<typename ComponentT, typename... ArgsT>
		[[nodiscard]]
		ComponentT* CreateComponent(ArgsT&&... args) {

			auto pool = component_pools.GetOrCreatePool<ComponentT>();

			void* memory = pool->CreateObject();

			ComponentT* component = new(memory) ComponentT(std::forward(args)...);

			return component;
		}


		void DestroyComponent(IComponent* component) {

			auto pool = component_pools.GetPool(component->GetTypeIDX());

			pool->DestroyComponent(reinterpret_cast<void*>(component));
		}


	private:
		ComponentPoolMap component_pools;
};
