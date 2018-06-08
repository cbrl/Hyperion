template<typename EntityT, typename... ArgsT>
[[nodiscard]]
handle64 ECS::createEntity(ArgsT&&... args) {
	static_assert(std::is_base_of_v<IEntity, EntityT>,
		"Calling ECS::CreateEntity() with non-entity type.");

	return entity_mgr->createEntity<EntityT>(std::forward<ArgsT>(args)...);
}


template<typename ComponentT, typename... ArgsT>
ComponentT* ECS::addComponent(handle64 entity, ArgsT&&... args) {
	static_assert(std::is_base_of_v<IComponent, ComponentT>,
		"Calling ECS::AddComponent() with non-component type.");

	return entity_mgr->getEntity(entity)->addComponent<ComponentT>(std::forward<ArgsT>(args)...);
}


template<typename ComponentT>
void ECS::removeComponent(handle64 entity) const {
	static_assert(std::is_base_of_v<IComponent, ComponentT>,
		"Calling ECS::RemoveComponent() with non-component type.");

	entity_mgr->getEntity(entity)->removeComponent<ComponentT>();
}


template<typename ComponentT>
[[nodiscard]]
ComponentT* ECS::getComponent(handle64 entity) const {
	static_assert(std::is_base_of_v<IComponent, ComponentT>,
		"Calling ECS::GetComponent() with non-component type.");

	return entity_mgr->getEntity(entity)->getComponent<ComponentT>();
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


// Do something to each active entity or component
template<typename T, typename ActionT>
void ECS::forEachActive(ActionT act) {
	if constexpr (std::is_base_of_v<IEntity, T>) {
		if (!entity_mgr->knowsEntity<T>()) return;

		for (auto entity = entity_mgr->begin<T>(); entity != entity_mgr->end<T>(); ++entity) {
			if (entity->isActive()) act(*entity);
		}
	}

	if constexpr (std::is_base_of_v<IComponent, T>) {
		if (!component_mgr->knowsComponent<T>()) return;

		for (auto component = component_mgr->begin<T>(); component != component_mgr->end<T>(); ++component) {
			if (component->isActive()) act(*component);
		}
	}
}
