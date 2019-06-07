
namespace ecs {

template<typename ComponentT, typename... ArgsT>
ComponentT& ComponentMgr::createComponent(ArgsT&&... args) {
	static_assert(std::is_base_of_v<IComponent, ComponentT>,
	              "Calling ComponentMgr::CreateComponent() with non-component type.");

	static_assert(std::is_constructible_v<ComponentT, ArgsT...>,
	              "Component does not have a constructor taking the provided argument types.");

	using pool_t = ResourcePool<ComponentT>;

	// Get or create the component pool
	const auto it = component_pools.find(ComponentT::index);
	if (it == component_pools.end()) {
		const auto pair = component_pools.try_emplace(ComponentT::index, std::make_unique<pool_t>());
		it = pair.first; //pair == pair<iterator, bool>
	}

	auto& pool      = *static_cast<pool_t*>(it->second.get());
	auto& component = pool.emplace_back(std::forward<ArgsT>(args)...);

	// Perform extra initialization steps if the component is an event participator
	if constexpr (std::is_base_of_v<EventParticipator, ComponentT>) {
		component.setEventMgr(gsl::make_not_null(&event_handler));
		if constexpr (std::is_base_of_v<EventListener, ComponentT>) {
			component.doRegisterCallbacks();
		}
	}

	return component;
}


template<typename ComponentT>
size_t ComponentMgr::countOf() {
	const auto it = component_pools.find(ComponentT::index);
	if (it == component_pools.end()) {
		return 0;
	}
	return it->second.size();
}


template<typename ComponentT>
bool ComponentMgr::knowsComponent() const {
	return component_pools.find(ComponentT::index) != component_pools.end();
}


template<typename ComponentT>
void ComponentMgr::forEach(const std::function<void(ComponentT&)>& act) {

	// Find the component pool
	const auto it = component_pools.find(ComponentT::index);
	if (it == component_pools.end()) {
		return;
	}

	// Apply the action to each component
	using pool_t = ResourcePool<ComponentT>;
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
	using pool_t = ResourcePool<ComponentT>;
	auto& pool = *static_cast<pool_t*>(it->second.get());
	for (const ComponentT& component : pool) {
		act(component);
	}
}

} // namespace ecs