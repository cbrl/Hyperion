module;

#include <concepts>
#include <functional>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <unordered_map>

#include "datatypes/scalar_types.h"
#include "memory/handle/handle.h"
#include "memory/resource_pool.h"

export module ecs:component;

import log;
import :event_mgr;


template<typename T>
std::type_index get_type_index() {
	return std::type_index{typeid(T)};
}


export namespace ecs {

class ComponentMgr;


//----------------------------------------------------------------------------------
// Component
//----------------------------------------------------------------------------------
//
// Base class that all components will derive from
//
//----------------------------------------------------------------------------------
class Component {
	friend class ComponentMgr;

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
protected:

	Component() = default;

public:

	Component(const Component& component) = delete;
	Component(Component&& component) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~Component() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Component& operator=(const Component& component) = delete;
	Component& operator=(Component&& component) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Get the component's type index
	[[nodiscard]]
	std::type_index getTypeIndex() const {
		// Will return the type info for the most derived class, as this is a polymorphic
		// class (due to the virtual destructor).
		return std::type_index{typeid(*this)};
	}

	// Get the ptr of the entity that owns this component
	[[nodiscard]]
	handle64 getOwner() const noexcept {
		return owner;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - State
	//----------------------------------------------------------------------------------

	// Set the component's state
	void setActive(bool state) {
		active = state;
	}

	// Get the component's state
	[[nodiscard]]
	bool isActive() const {
		return active;
	}

private:

	void setOwner(handle64 owner_handle) {
		owner = owner_handle;
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Is the component active?
	bool active = true;

	// The entity that owns this component. Set on creation in IEntity.
	handle64 owner;
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
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ComponentMgr(EventMgr& event_mgr) : event_mgr(event_mgr) {
	}

	ComponentMgr(const ComponentMgr& manager) = delete;
	ComponentMgr(ComponentMgr&& manager) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~ComponentMgr() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ComponentMgr& operator=(const ComponentMgr& manager) = delete;
	ComponentMgr& operator=(ComponentMgr&& manager) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Components
	//----------------------------------------------------------------------------------

	// Construct a component and add it to the given entity
	template<typename ComponentT, typename... ArgsT>
	requires std::derived_from<ComponentT, Component> && std::constructible_from<ComponentT, ArgsT...>
	[[nodiscard]]
	ComponentT& add(handle64 entity, ArgsT&&... args) {
		using pool_t = ResourcePool<handle64::value_type, ComponentT>;

		// Get or create the component pool
		auto it = component_pools.find(get_type_index<ComponentT>());
		if (it == component_pools.end()) {
			const auto pair = component_pools.try_emplace(get_type_index<ComponentT>(), std::make_unique<pool_t>());
			it = pair.first; //pair == pair<iterator, bool>
		}

		auto& pool = *static_cast<pool_t*>(it->second.get());
		auto& component = pool.construct(entity.index, std::forward<ArgsT>(args)...);

		// Setup the component
		component.setOwner(entity);

		return component;
	}

	// Destroy a given component. The component won't actually be destroyed until the end of the ECS update.
	template<typename ComponentT>
	void remove(handle64 entity) {
		if (const auto it = component_pools.find(get_type_index<ComponentT>()); it != component_pools.end()) {
			if (it->second->contains(entity.index)) {
				expired_components[get_type_index<ComponentT>()].push_back(entity);
			}
			else {
				Logger::log(LogLevel::err, "Attempting to remove a component ({}) from an entity that does not contain it", get_type_index<ComponentT>().name());
				assert(false);
			}
		}
	}

	// Destroy a given component. The component won't actually be destroyed until the end of the ECS update.
	void remove(handle64 entity, Component& component) {
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
	void removeAll(handle64 entity) {
		for (const auto& [index, pool] : component_pools) {
			if (pool->contains(entity.index)) {
				expired_components[index].push_back(entity);
			}
		}
	}

	// Remove all components that were passed to destroyComponent()
	void removeExpiredComponents(){
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
	bool has(handle64 entity) const noexcept {
		using pool_t = ResourcePool<handle64::value_type, ComponentT>;

		if (const auto it = component_pools.find(get_type_index<ComponentT>()); it != component_pools.end()) {
			auto& pool = *static_cast<pool_t*>(it->second.get());
			return pool.contains(entity.index);
		}
		return false;
	}

	// Get a component of type ComponentT owned by the given entity
	template<typename ComponentT>
	[[nodiscard]]
	ComponentT& get(handle64 entity) {
		return const_cast<ComponentT&>(std::as_const(*this).get<ComponentT>(entity));
	}

	// Get a component of type ComponentT owned by the given entity
	template<typename ComponentT>
	[[nodiscard]]
	const ComponentT& get(handle64 entity) const {
		using pool_t = ResourcePool<handle64::value_type, ComponentT>;

		const auto& pool = *static_cast<const pool_t*>(component_pools.at(get_type_index<ComponentT>()).get());
		return pool.get(entity.index);
	}

	// Attempt to get a component of type ComponentT owned by the give entity.
	// Returns nullptr if the entity does not own the specified component.
	template<typename ComponentT>
	[[nodiscard]]
	ComponentT* tryGet(handle64 entity) {
		return const_cast<ComponentT*>(std::as_const(*this).tryGet<ComponentT>(entity));
	}

	// Attempt to get a component of type ComponentT owned by the give entity.
	// Returns nullptr if the entity does not own the specified component.
	template<typename ComponentT>
	[[nodiscard]]
	const ComponentT* tryGet(handle64 entity) const {
		using pool_t = ResourcePool<handle64::value_type, ComponentT>;

		if (const auto it = component_pools.find(get_type_index<ComponentT>()); it != component_pools.end()) {
			const auto& pool = *static_cast<const pool_t*>(it->second.get());
			if (pool.contains(entity.index))
				return &get<ComponentT>(entity);
		}
		return nullptr;
	}

	// Get the number of the specified component
	template<typename ComponentT>
	[[nodiscard]]
	size_t count() const noexcept {
		const auto it = component_pools.find(get_type_index<ComponentT>());
		if (it == component_pools.end()) {
			return 0;
		}
		return it->second.size();
	}

	// Check if the component manager has a pool of this component type
	template<typename ComponentT>
	[[nodiscard]]
	bool knowsComponent() const noexcept {
		return component_pools.contains(get_type_index<ComponentT>());
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Iteration
	//----------------------------------------------------------------------------------

	// Apply an action to each component
	template<typename ComponentT>
	void forEach(const std::function<void(ComponentT&)>& act) {
		// Call the const version of forEach, but cast the const component references back
		// to mutable references and pass them to the user-provided function.
		std::as_const(*this).forEach<ComponentT>([&act](const ComponentT& component) {
			act(const_cast<ComponentT&>(component));
		});
	}

	// Apply an action to each component
	template<typename ComponentT>
	void forEach(const std::function<void(const ComponentT&)>& act) const {
		// Find the component pool
		const auto it = component_pools.find(get_type_index<ComponentT>());
		if (it == component_pools.end()) {
			return;
		}

		// Apply the action to each component
		using pool_t = ResourcePool<handle64::value_type, ComponentT>;
		auto& pool = *static_cast<pool_t*>(it->second.get());
		for (const ComponentT& component : pool) {
			act(component);
		}
	}

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
