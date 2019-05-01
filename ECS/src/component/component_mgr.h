#pragma once

#include "memory/resource_pool.h"
#include "component/component.h"


namespace ecs {

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
	ComponentT& createComponent(ArgsT&&... args);

	void destroyComponent(IComponent& component) {
		const auto it = component_pools.find(component.getTypeIndex());
		if (it != component_pools.end()) {
			it->second->remove_resource(&component);
		}
		else {
			Logger::log(LogLevel::err,
			            "Could not find appropriate pool when destroying a component of type \"{}\"",
			            component.getTypeIndex().name());
		}
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

	// A reference to the event manager. Passed to systems that inherit from EventListener.
	EventMgr& event_handler;

	// Map of unique resource pools for each type of component
	std::unordered_map<std::type_index, std::unique_ptr<IResourcePool>> component_pools;
};

} // namespace ecs

#include "component_mgr.tpp"