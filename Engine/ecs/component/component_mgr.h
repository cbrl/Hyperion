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

			auto pool = component_pools.GetOrCreatePool<ComponentT>();

			void* memory = pool->CreateObject();

			ComponentT* component = new(memory) ComponentT(std::forward(args)...);

			return component;
		}


		void DestroyComponent(IComponent* component) {

			auto pool = component_pools.GetPool(component->GetTypeID());

			pool->DestroyObject(reinterpret_cast<void*>(component));
		}


	private:
		// Map of unique resource pools for each type of component
		ResourcePoolMap component_pools;
};
