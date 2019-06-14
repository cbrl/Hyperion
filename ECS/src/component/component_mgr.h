#pragma once

#include "memory/resource_pool.h"
#include "component/component.h"


namespace ecs {

class EventMgr;

/*
template<typename EntityT, typename ComponentT>
class ComponentPool : public SparseSet<EntityT> {
public:
	using base_type              = SparseSet<EntityT>;
	using container_type         = std::vector<ComponentT>;
	using entity_type            = EntityT;
	using value_type             = ComponentT;
	using pointer                = ComponentT*;
	using const_pointer          = const ComponentT*;
	using reference              = ComponentT&;
	using const_reference        = const ComponentT&;
	using size_type              = size_t;
	using difference_type        = ptrdiff_t;
	using iterator               = typename container_type::iterator;
	using const_iterator         = typename container_type::const_iterator;
	using reverse_iterator       = typename container_type::reverse_iterator;
	using const_reverse_iterator = typename container_type::const_reverse_iterator;

	void reserve(size_type new_cap) {
		base_type::reserve(new_cap);
		components.reserve(new_cap);
	}

	template<typename... ArgsT>
	[[nodiscard]]
	reference construct(entity_type entity, ArgsT&& ... args) {
		// TODO: error if entity is already present (base_type::contains(entity))
		components.emplace_back(std::forward<ArgsT>(args)...);
		base_type::insert(entity);
		return components.back();
	}

	void destroy(entity_type entity) {
		auto&& back = std::move(components.back());
		components[base_type::indexOf(entity)] = std::move(back);
		components.pop_back();
		base_type::erase(entity);
	}

private:

	container_type components;
};
*/

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

	// Destroy a given component. The component won't actually be destroyed until the end of the ECS update.
	void destroyComponent(IComponent& component) {
		expired_components[component.getTypeIndex()].push_back(&component);
	}

	// Remove all components that were passed to destroyComponent()
	void removeExpiredComponents() {
		for (auto& [type, vec] : expired_components) { //for each vector of a component type
			for (auto* ptr : vec) { //for each component in the vector

				// Destroy the component if an associated pool exists
				const auto it = component_pools.find(ptr->getTypeIndex());
				if (it != component_pools.end()) {
					it->second->remove_resource(ptr);
				}
				else {
					Logger::log(
						LogLevel::err,
						"Could not find appropriate pool when destroying a component of type \"{}\"",
						ptr->getTypeIndex().name()
					);
				}
			}

			// Clear the vector after all components have been processed
			vec.clear();
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
	std::unordered_map<std::type_index, std::unique_ptr<IResourcePool>> component_pools;

	// A map of vectors containing components that need to be destroyed
	std::unordered_map<std::type_index, std::vector<IComponent*>> expired_components;
};

} // namespace ecs

#include "component_mgr.tpp"