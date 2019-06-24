#pragma once

#include "component/component.h"
#include "memory/resource_pool.h"
#include "datatypes/container_types.h"


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
	ComponentMgr(EventMgr& handler)
		: event_handler(handler) {
	}

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
	ComponentT& createComponent(handle64 entity, ArgsT&&... args);

	template<typename ComponentT>
	void destroyComponent(handle64 entity);

	// Destroy a given component. The component won't actually be destroyed until the end of the ECS update.
	void destroyComponent(handle64 entity, IComponent& component) {
		const auto it = component_pools.find(component.getTypeIndex());
		if (it != component_pools.end()) {
			if (it->second->contains(entity.index)) {
				expired_components[component.getTypeIndex()].push_back(entity);
			}
			else {
				Logger::log(LogLevel::err, "Attempting to remove a component from an entity that does not contain it");
				assert(false);
			}
		}
	}

	// Destroy all components owned by the given entity
	void destroyAll(handle64 entity) {
		for (const auto& [index, pool] : component_pools) {
			if (pool->contains(entity.index)) {
				expired_components[index].push_back(entity);
			}
		}
	}

	// Remove all components that were passed to destroyComponent()
	void removeExpiredComponents() {
		for (auto& [type, vec] : expired_components) { //for each vector of a component type
			const auto it = component_pools.find(type);

			if (it != component_pools.end()) { //if the associated pool exists
				for (auto& handle : vec) { //destroy each entity's component
					it->second->erase(handle.index);
				}				
			}
			else {
				Logger::log(
					LogLevel::err,
					"Could not find appropriate pool when destroying a component of type \"{}\"",
					type.name()
				);
				assert(false);
			}

			// Clear the vector after all components have been processed
			vec.clear();
		}
	}

	// Check if the given entity owns a component of the specified type
	template<typename ComponentT>
	[[nodiscard]]
	bool hasComponent(handle64 entity) const noexcept;

	// Get a component of type ComponentT owned by the given entity
	template<typename ComponentT>
	[[nodiscard]]
	ComponentT& getComponent(handle64 entity);

	// Get a component of type ComponentT owned by the given entity
	template<typename ComponentT>
	[[nodiscard]]
	const ComponentT& getComponent(handle64 entity) const;

	// Attempt to get a component of type ComponentT owned by the give entity.
	// Returns nullptr if the entity does not own the specified component.
	template<typename ComponentT>
	[[nodiscard]]
	ComponentT* tryGetComponent(handle64 entity);

	// Attempt to get a component of type ComponentT owned by the give entity.
	// Returns nullptr if the entity does not own the specified component.
	template<typename ComponentT>
	[[nodiscard]]
	const ComponentT* tryGetComponent(handle64 entity) const;

	// Get the number of the specified component
	template<typename ComponentT>
	[[nodiscard]]
	size_t countOf() const noexcept;

	// Check if the component manager has a pool of this component type
	template<typename ComponentT>
	[[nodiscard]]
	bool knowsComponent() const noexcept;


	//----------------------------------------------------------------------------------
	// Member Functions - Iteration
	//----------------------------------------------------------------------------------

	// Apply an action to each component
	template<typename ComponentT>
	void forEach(const std::function<void(ComponentT&)>& act);

	// Apply an action to each component
	template<typename ComponentT>
	void forEach(const std::function<void(const ComponentT&)>& act) const;

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// A reference to the event manager. Passed to systems that inherit from EventListener.
	EventMgr& event_handler;

	// Map of unique resource pools for each type of component
	std::unordered_map<std::type_index, std::unique_ptr<IResourcePool<handle64::value_type>>> component_pools;

	// A map of vectors containing components that need to be destroyed
	std::unordered_map<std::type_index, std::vector<handle64>> expired_components;
};

} // namespace ecs

#include "component_mgr.tpp"