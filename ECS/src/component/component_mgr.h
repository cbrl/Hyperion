#pragma once

#include "memory/resource_pool.h"
#include "memory/sparse_set.h"
#include "component/component.h"


namespace ecs {

class EventMgr;


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
	using iterator               = typename container_type::reverse_iterator;
	using const_iterator         = typename container_type::const_reverse_iterator;

	//----------------------------------------------------------------------------------
	// Member Functions - Modifiers
	//----------------------------------------------------------------------------------
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
		components[base_type::index_of(entity)] = std::move(back);
		components.pop_back();
		base_type::erase(entity);
	}

	void clear() noexcept {
		base_type::clear();
		components.clear();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Access
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	bool contains(const entity_type& val) const noexcept {
		return base_type::contains(val);
	}

	[[nodiscard]]
	reference get(const entity_type& val) {
		return components[base_type::index_of(val)];
	}

	[[nodiscard]]
	const_reference get(const entity_type& val) const {
		return components[base_type::index_of(val)];
	}

	[[nodiscard]]
	pointer data() noexcept {
		return components.data();
	}

	[[nodiscard]]
	const_pointer data() const noexcept {
		return components.data();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Iterators
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	iterator begin() noexcept {
		return components.rbegin();
	}

	[[nodiscard]]
	const_iterator begin() const noexcept {
		return components.rbegin();
	}

	[[nodiscard]]
	const_iterator cbegin() const noexcept {
		return components.crbegin();
	}

	[[nodiscard]]
	iterator end() noexcept {
		return components.rend();
	}

	[[nodiscard]]
	const_iterator end() const noexcept {
		return components.rend();
	}

	[[nodiscard]]
	const_iterator cend() const noexcept {
		return components.crend();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Capacity
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	bool empty() const noexcept {
		return components.empty();
	}

	[[nodiscard]]
	size_type size() const noexcept {
		return components.size();
	}

	void reserve(size_type new_cap) {
		base_type::reserve(new_cap);
		components.reserve(new_cap);
	}

	void shrink_to_fit() {
		base_type::shrink_to_fit();
		components.shrink_to_fit();
	}

private:

	container_type components;
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