#include "event/event_mgr.h"

namespace ecs {

template<typename ComponentT, typename... ArgsT>
ComponentT& ComponentMgr::add(handle64 entity, ArgsT&&... args) {
	static_assert(std::is_base_of_v<IComponent, ComponentT>, "ComponentT must inherit from Component.");

	static_assert(std::is_constructible_v<ComponentT, ArgsT...>,
	              "ComponentT does not have a constructor taking the provided argument types.");

	using pool_t = ResourcePool<handle64::value_type, ComponentT>;

	// Get or create the component pool
	auto it = component_pools.find(ComponentT::index);
	if (it == component_pools.end()) {
		const auto pair = component_pools.try_emplace(ComponentT::index, std::make_unique<pool_t>());
		it = pair.first; //pair == pair<iterator, bool>
	}

	auto& pool      = *static_cast<pool_t*>(it->second.get());
	auto& component = pool.construct(entity.index, std::forward<ArgsT>(args)...);

	// Setup the component
	component.setOwner(entity);

	return component;
}


template<typename ComponentT>
void ComponentMgr::remove(handle64 entity) {
	if (const auto it = component_pools.find(ComponentT::index); it != component_pools.end()) {
		if (it->second->contains(entity.index)) {
			expired_components[ComponentT::index].push_back(entity);
		}
		else {
			Logger::log(LogLevel::err, "Attempting to remove a component from an entity that does not contain it");
			assert(false);
		}
	}
}



template<typename ComponentT>
bool ComponentMgr::has(handle64 entity) const noexcept {
	using pool_t = ResourcePool<handle64::value_type, ComponentT>;

	if (const auto it = component_pools.find(ComponentT::index); it != component_pools.end()) {
		auto& pool = *static_cast<pool_t*>(it->second.get());
		return pool.contains(entity.index);
	}
	return false;
}


template<typename ComponentT>
[[nodiscard]]
ComponentT& ComponentMgr::get(handle64 entity) {
	using pool_t = ResourcePool<handle64::value_type, ComponentT>;

	auto& pool = *static_cast<pool_t*>(component_pools.at(ComponentT::index).get());
	return pool.get(entity.index);
}

template<typename ComponentT>
[[nodiscard]]
const ComponentT& ComponentMgr::get(handle64 entity) const {
	using pool_t = ResourcePool<handle64::value_type, ComponentT>;

	const auto& pool = *static_cast<const pool_t*>(component_pools.at(ComponentT::index).get());
	return pool.get(entity.index);
}


template<typename ComponentT>
[[nodiscard]]
ComponentT* ComponentMgr::tryGet(handle64 entity) {
	using pool_t = ResourcePool<handle64::value_type, ComponentT>;

	if (const auto it = component_pools.find(ComponentT::index); it != component_pools.end()) {
		const auto& pool = *static_cast<const pool_t*>(it->second.get());
		if (pool.contains(entity.index))
			return &get<ComponentT>(entity);
	}
	return nullptr;
}


template<typename ComponentT>
[[nodiscard]]
const ComponentT* ComponentMgr::tryGet(handle64 entity) const {
	using pool_t = ResourcePool<handle64::value_type, ComponentT>;

	if (const auto it = component_pools.find(ComponentT::index); it != component_pools.end()) {
		const auto& pool = *static_cast<const pool_t*>(it->second.get());
		if (pool.contains(entity.index))
			return &get<ComponentT>(entity);
	}
	return nullptr;
}


template<typename ComponentT>
size_t ComponentMgr::count() const noexcept {
	const auto it = component_pools.find(ComponentT::index);
	if (it == component_pools.end()) {
		return 0;
	}
	return it->second.size();
}


template<typename ComponentT>
bool ComponentMgr::knowsComponent() const noexcept {
	return component_pools.contains(ComponentT::index);
}


template<typename ComponentT>
void ComponentMgr::forEach(const std::function<void(ComponentT&)>& act) {

	// Find the component pool
	const auto it = component_pools.find(ComponentT::index);
	if (it == component_pools.end()) {
		return;
	}

	// Apply the action to each component
	using pool_t = ResourcePool<handle64::value_type, ComponentT>;
	auto& pool = *static_cast<pool_t*>(it->second.get());
	for (ComponentT& component : pool) {
		act(component);
	}
}


template<typename ComponentT>
void ComponentMgr::forEach(const std::function<void(const ComponentT&)>& act) const {

	// Find the component pool
	const auto it = component_pools.find(ComponentT::index);
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

} // namespace ecs