
template<typename EntityT, typename... ArgsT>
[[nodiscard]]
const Handle64 ECS::CreateEntity(ArgsT&&... args) {
	static_assert(std::is_base_of_v<IEntity, EntityT>,
				  "Calling ECS::CreateEntity() with non-entity type.");

	return entity_mgr->CreateEntity<EntityT>(std::forward<ArgsT>(args)...);
}


template<typename ComponentT, typename... ArgsT>
ComponentT* const ECS::AddComponent(Handle64 entity, ArgsT&&... args) {
	static_assert(std::is_base_of_v<IComponent, ComponentT>,
				  "Calling ECS::AddComponent() with non-component type.");

	entity_mgr->GetEntity(entity)->AddComponent<ComponentT>(std::forward<ArgsT>(args)...);
}


template<typename ComponentT>
void ECS::RemoveComponent(Handle64 entity) {
	static_assert(std::is_base_of_v<IComponent, ComponentT>,
				  "Calling ECS::RemoveComponent() with non-component type.");

	entity_mgr->GetEntity(entity)->RemoveComponent<ComponentT>();
}


template<typename ComponentT>
[[nodiscard]]
ComponentT* const ECS::GetComponent(Handle64 entity) {
	static_assert(std::is_base_of_v<IComponent, ComponentT>,
				  "Calling ECS::GetComponent() with non-component type.");

	return entity_mgr->GetEntity(entity)->GetComponent<ComponentT>();
}


template<typename SystemT, typename... ArgsT>
SystemT* const ECS::AddSystem(ArgsT&&... args) {
	static_assert(std::is_base_of_v<ISystem, SystemT>,
				  "Calling ECS::AddSystem() with non-system type.");

	return system_mgr->AddSystem<SystemT>(std::forward<ArgsT>(args)...);
}


template<typename T>
size_t ECS::CountOf() const {
	if constexpr (std::is_base_of_v<IEntity, T>) {
		return entity_mgr->CountOf<T>();
	}
	if constexpr (std::is_base_of_v<IComponent, T>) {
		return component_mgr->CountOf<T>();
	}
}


template<typename T>
size_t ECS::CountOfActive() const {
	size_t count = 0;

	if constexpr (std::is_base_of_v<IEntity, T>) {
		return entity_mgr->ForEachActive<T>([&count]() {
			++count;
		});
	}
	if constexpr (std::is_base_of_v<IComponent, T>) {
		return component_mgr->ForEachActive<T>([&count]() {
			++count;
		});
	}
}


// Do something to each active entity or component
template<typename T, typename ActionT>
void ECS::ForEachActive(ActionT act) {
	if constexpr (std::is_base_of_v<IEntity, T>) {
		if (!entity_mgr->KnowsEntity<T>()) return;

		for (auto entity = entity_mgr->begin<T>(); entity != entity_mgr->end<T>(); ++entity) {
			if (entity->IsActive()) act(*entity);
		}
	}

	if constexpr (std::is_base_of_v<IComponent, T>) {
		if (!component_mgr->KnowsComponent<T>()) return;

		for (auto component = component_mgr->begin<T>(); component != component_mgr->end<T>(); ++component) {
			if (component->IsActive()) act(*component);
		}
	}
}