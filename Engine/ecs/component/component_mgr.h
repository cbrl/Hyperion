#pragma once

#include "util\memory\resource_pool.h"
#include "ecs\component\component.h"



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

			static_assert(std::is_base_of_v<IComponent, ComponentT>,
						  "Calling ComponentMgr::CreateComponent() with non-component type.");

			// Get the pool for this component type
			auto pool = component_pools.GetOrCreatePool<ComponentT>();

			// Allocate memory
			void* memory = pool->AllocateObject();

			// Construct the component
			ComponentT* component = new(memory) ComponentT(std::forward<ArgsT>(args)...);

			return component;
		}


		void DestroyComponent(IComponent* component) {

			auto pool = component_pools.GetPool(component->GetTypeID());

			pool->DestroyObject(reinterpret_cast<void*>(component));
		}


		template<typename ComponentT>
		bool KnowsComponent() const {
			return component_pools.PoolExists<ComponentT>();
		}


		template<typename ComponentT>
		typename ResourcePool<ComponentT>::iterator begin() {
			using pool_t = ResourcePool<ComponentT>;
			auto* pool = static_cast<pool_t*>(component_pools.GetPool<ComponentT>());
			return pool->begin();
		}

		template<typename ComponentT>
		typename ResourcePool<ComponentT>::iterator end() {
			using pool_t = ResourcePool<ComponentT>;
			auto* pool = static_cast<pool_t*>(component_pools.GetPool<ComponentT>());
			return pool->end();
		}


	private:
		// Map of unique resource pools for each type of component
		ResourcePoolMap component_pools;
};
