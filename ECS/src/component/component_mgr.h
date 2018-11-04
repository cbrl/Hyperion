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
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	ComponentMgr() = default;
	ComponentMgr(const ComponentMgr& manager) = delete;
	ComponentMgr(ComponentMgr&& manager) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~ComponentMgr() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	ComponentMgr& operator=(const ComponentMgr& manager) = delete;
	ComponentMgr& operator=(ComponentMgr&& manager) = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Components
	//----------------------------------------------------------------------------------

	template<typename ComponentT, typename... ArgsT>
	[[nodiscard]]
	ComponentT* createComponent(ArgsT&&... args) {
		static_assert(std::is_base_of_v<IComponent, ComponentT>,
			"Calling ComponentMgr::CreateComponent() with non-component type.");

		auto* pool = component_pools.getOrCreatePool<ComponentT>();
		ComponentT* component = pool->construct(std::forward<ArgsT>(args)...);

		return component;
	}


	void destroyComponent(IComponent* component) {
		auto* pool = component_pools.getPool(component->getTypeIndex());
		pool->deallocate(component);
	}


	// Get the number of the specified component
	template<typename ComponentT>
	size_t countOf() {
		return component_pools.poolExists<ComponentT>() ? component_pools.getPool<ComponentT>()->GetCount() : 0;
	}


	// Check if the component manager has created a component of this type
	template<typename ComponentT>
	bool knowsComponent() const {
		return component_pools.poolExists<ComponentT>();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Iteration
	//----------------------------------------------------------------------------------

	// Apply an action to each component
	template<typename ComponentT, typename ActionT>
	void forEach(ActionT&& act) {
		using pool_t = ResourcePool<ComponentT>;
		auto* pool = static_cast<pool_t*>(component_pools.getPool<ComponentT>());
		if (pool) {
			pool->forEach(act);
		}
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Map of unique resource pools for each type of component
	ResourcePoolManager component_pools;
};
