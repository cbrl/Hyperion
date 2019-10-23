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
	ComponentMgr(EventMgr& event_mgr);
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

	// Construct a component and add it to the given entity
	template<typename ComponentT, typename... ArgsT>
	requires std::derived_from<ComponentT, IComponent> && std::constructible_from<ComponentT, ArgsT...>
	[[nodiscard]]
	ComponentT& add(handle64 entity, ArgsT&&... args);

	// Destroy a given component. The component won't actually be destroyed until the end of the ECS update.
	template<typename ComponentT>
	void remove(handle64 entity);

	// Destroy a given component. The component won't actually be destroyed until the end of the ECS update.
	void remove(handle64 entity, IComponent& component);

	// Destroy all components owned by the given entity
	void removeAll(handle64 entity);

	// Remove all components that were passed to destroyComponent()
	void removeExpiredComponents();

	// Check if the given entity owns a component of the specified type
	template<typename ComponentT>
	[[nodiscard]]
	bool has(handle64 entity) const noexcept;

	// Get a component of type ComponentT owned by the given entity
	template<typename ComponentT>
	[[nodiscard]]
	ComponentT& get(handle64 entity);

	// Get a component of type ComponentT owned by the given entity
	template<typename ComponentT>
	[[nodiscard]]
	const ComponentT& get(handle64 entity) const;

	// Attempt to get a component of type ComponentT owned by the give entity.
	// Returns nullptr if the entity does not own the specified component.
	template<typename ComponentT>
	[[nodiscard]]
	ComponentT* tryGet(handle64 entity);

	// Attempt to get a component of type ComponentT owned by the give entity.
	// Returns nullptr if the entity does not own the specified component.
	template<typename ComponentT>
	[[nodiscard]]
	const ComponentT* tryGet(handle64 entity) const;

	// Get the number of the specified component
	template<typename ComponentT>
	[[nodiscard]]
	size_t count() const noexcept;

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

	// A reference to the event manager
	std::reference_wrapper<EventMgr> event_mgr;

	// Map of unique resource pools for each type of component
	std::unordered_map<std::type_index, std::unique_ptr<IResourcePool<handle64::value_type>>> component_pools;

	// A map of vectors containing components that need to be destroyed
	std::unordered_map<std::type_index, std::vector<handle64>> expired_components;
};

} // namespace ecs

#include "component_mgr.tpp"