#pragma once

#include "memory/resource_pool.h"
#include "component/component.h"


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
	ComponentT* createComponent(ArgsT&&... args) {

		static_assert(std::is_base_of_v<IComponent, ComponentT>,
			"Calling ComponentMgr::CreateComponent() with non-component type.");

		// Get the pool for this component type
		auto pool = component_pools.getOrCreatePool<ComponentT>();

		// Allocate memory
		void* memory = pool->allocateObject();

		// Construct the component
		ComponentT* component = new(memory) ComponentT(std::forward<ArgsT>(args)...);

		return component;
	}


	void destroyComponent(IComponent* component) {

		auto pool = component_pools.getPool(component->getTypeId());

		pool->destroyObject(reinterpret_cast<void*>(component));
	}


	template<typename ComponentT>
	size_t countOf() {
		return component_pools.poolExists<ComponentT>() ? component_pools.getPool<ComponentT>()->GetCount() : 0;
	}


	template<typename ComponentT>
	bool knowsComponent() const {
		return component_pools.poolExists<ComponentT>();
	}


	template<typename ComponentT>
	typename ResourcePool<ComponentT>::iterator begin() {
		using pool_t = ResourcePool<ComponentT>;
		auto* pool = static_cast<pool_t*>(component_pools.getPool<ComponentT>());
		return pool->begin();
	}

	template<typename ComponentT>
	typename ResourcePool<ComponentT>::iterator end() {
		using pool_t = ResourcePool<ComponentT>;
		auto* pool = static_cast<pool_t*>(component_pools.getPool<ComponentT>());
		return pool->end();
	}


private:
	// Map of unique resource pools for each type of component
	ResourcePoolFactory component_pools;
};
