template <typename ComponentT, typename... ArgsT>
ComponentT* ComponentMgr::createComponent(ArgsT&&... args) {
	static_assert(std::is_base_of_v<IComponent, ComponentT>,
	              "Calling ComponentMgr::CreateComponent() with non-component type.");

	static_assert(std::is_constructible_v<ComponentT, ArgsT...>,
	              "Component does not have a constructor taking the provided argument types.");

	auto* pool = component_pools.getOrCreatePool<ComponentT>();
	auto& component = pool->emplace_back(std::forward<ArgsT>(args)...);

	if constexpr (std::is_base_of_v<EventParticipator, ComponentT>) {
		component.setEventMgr(gsl::make_not_null(&event_handler));
		if constexpr (std::is_base_of_v<EventListener, ComponentT>) {
			component.registerCallbacks();
		}
	}

	return &component;
}


template <typename ComponentT>
size_t ComponentMgr::countOf() {
	return component_pools.poolExists<ComponentT>() ? component_pools.getPool<ComponentT>()->GetCount() : 0;
}


template <typename ComponentT>
bool ComponentMgr::knowsComponent() const {
	return component_pools.poolExists<ComponentT>();
}


template <typename ComponentT, typename ActionT>
void ComponentMgr::forEach(ActionT&& act) {
	using pool_t = ResourcePool<ComponentT>;
	auto* pool = static_cast<pool_t*>(component_pools.getPool<ComponentT>());
	if (!pool) return;
	for (auto& component : *pool) {
		act(component);
	}
}