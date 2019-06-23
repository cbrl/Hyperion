#pragma once

#include "memory/resource_pool.h"
#include "memory/sparse_set.h"
#include "component/component.h"


namespace ecs {

class EventMgr;

template<typename HandleT, typename ComponentT>
class ComponentPool final : public SparseSet<HandleT> {
public:
	using base_type              = SparseSet<HandleT>;
	using container_type         = std::vector<ComponentT>;
	using handle_type            = HandleT;
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
	reference construct(handle_type entity_idx, ArgsT&& ... args) {
		// TODO: error if entity is already present (base_type::contains(entity_idx))
		components.emplace_back(std::forward<ArgsT>(args)...);
		base_type::insert(entity_idx);
		return components.back();
	}

	void erase(handle_type entity_idx) override {
		auto&& back = std::move(components.back());
		components[base_type::index_of(entity_idx)] = std::move(back);
		components.pop_back();
		base_type::erase(entity_idx);
	}

	void clear() noexcept override {
		base_type::clear();
		components.clear();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Access
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	bool contains(handle_type val) const noexcept {
		return base_type::contains(val);
	}

	[[nodiscard]]
	reference get(handle_type val) {
		return components[base_type::index_of(val)];
	}

	[[nodiscard]]
	const_reference get(handle_type val) const {
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

	void reserve(size_type new_cap) override {
		base_type::reserve(new_cap);
		components.reserve(new_cap);
	}

	void shrink_to_fit() override {
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
		if (component_pools.contains(component.getTypeIndex())) {
			expired_components[component.getTypeIndex()].push_back(entity);
		}
		else {
			Logger::log(LogLevel::err, "Attempting to remove a component from an entity that does not contain it");
			assert(false);
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
	size_t countOf() const noexcept;

	// Check if the component manager has a pool of this component type
	template<typename ComponentT>
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
	std::unordered_map<std::type_index, std::unique_ptr<SparseSet<handle64::value_type>>> component_pools;

	// A map of vectors containing components that need to be destroyed
	std::unordered_map<std::type_index, std::vector<handle64>> expired_components;
};

} // namespace ecs

#include "component_mgr.tpp"