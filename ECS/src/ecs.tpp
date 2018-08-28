template<typename EntityT, typename... ArgsT>
[[nodiscard]]
EntityPtr ECS::createEntity(ArgsT&&... args) {
	static_assert(std::is_base_of_v<IEntity, EntityT>,
		"Calling ECS::CreateEntity() with non-entity type.");

	return entity_mgr->createEntity<EntityT>(std::forward<ArgsT>(args)...);
}


template<typename SystemT, typename... ArgsT>
SystemT* ECS::addSystem(ArgsT&&... args) {
	static_assert(std::is_base_of_v<ISystem, SystemT>,
		"Calling ECS::AddSystem() with non-system type.");

	return system_mgr->addSystem<SystemT>(std::forward<ArgsT>(args)...);
}


template<typename T>
size_t ECS::countOf() const {
	if constexpr (std::is_base_of_v<IEntity, T>) {
		return entity_mgr->countOf<T>();
	}
	if constexpr (std::is_base_of_v<IComponent, T>) {
		return component_mgr->countOf<T>();
	}
	return 0;
}


// Do something for each entity or component
template<typename T, typename ActionT>
void ECS::forEach(ActionT&& act) {
	if constexpr (std::is_base_of_v<IEntity, T>) {
		if (!entity_mgr->knowsEntity<T>()) return;

		entity_mgr->forEach<T>(act);
	}

	if constexpr (std::is_base_of_v<IComponent, T>) {
		if (!component_mgr->knowsComponent<T>()) return;

		component_mgr->forEach<T>(act);
	}
}
