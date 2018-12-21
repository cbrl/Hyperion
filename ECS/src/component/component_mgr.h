#pragma once

#include "memory/resource_pool.h"
#include "component/component.h"


class EventMgr;


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

	ComponentMgr(EventMgr& handler) : event_handler(handler) {}
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
	ComponentT* createComponent(ArgsT&&... args);


	void destroyComponent(IComponent* component) {
		auto* pool = component_pools.getPool(component->getTypeIndex());
		pool->remove_resource(component);
	}


	// Get the number of the specified component
	template<typename ComponentT>
	size_t countOf();


	// Check if the component manager has created a component of this type
	template<typename ComponentT>
	bool knowsComponent() const;


	//----------------------------------------------------------------------------------
	// Member Functions - Iteration
	//----------------------------------------------------------------------------------

	// Apply an action to each component
	template<typename ComponentT, typename ActionT>
	void forEach(ActionT&& act);


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Map of unique resource pools for each type of component
	ResourcePoolManager component_pools;

	// A reference to the event manager. Passed to systems that inherit from EventListener.
	EventMgr& event_handler;
};

#include "component_mgr.tpp"
