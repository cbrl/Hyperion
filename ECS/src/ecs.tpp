
namespace ecs {

template<typename SystemT, typename... ArgsT>
SystemT& ECS::addSystem(ArgsT&&... args) {
	static_assert(std::is_base_of_v<ISystem, SystemT>,
		"Calling ECS::AddSystem() with non-system type.");

	return system_mgr->addSystem<SystemT>(std::forward<ArgsT>(args)...);
}


template<typename SystemT>
void ECS::removeSystem() {
	system_mgr->removeSystem<SystemT>();
}


template<typename SystemT>
SystemT* ECS::getSystem() const {
	return system_mgr->getSystem<SystemT>();
}

template<typename SystemT>
void ECS::setSystemPriority(u32 priority) {
	system_mgr->setSystemPriority<SystemT>(priority);
}


template<typename EventT, typename... ArgsT>
void ECS::sendEvent(ArgsT&&... args) {
	event_mgr->send<EventT>(std::forward<ArgsT>(args)...);
}


template<typename T>
size_t ECS::countOf() const {
	if constexpr (std::is_same_v<Entity, T>) {
		return entity_mgr->count();
	}
	if constexpr (std::is_base_of_v<IComponent, T>) {
		return component_mgr->countOf<T>();
	}
	return 0;
}


template<typename... ComponentT>
void ECS::forEach(const std::function<void(Entity&)>& act) {

	static_assert((std::is_base_of_v<IComponent, ComponentT> && ...));

	if constexpr (sizeof...(ComponentT) == 0) {
		entity_mgr->forEach(act);
	}
	else {
		if ((!component_mgr->knowsComponent<ComponentT>() || ...))
			return;

		std::vector<std::reference_wrapper<Entity>> entities;

		entity_mgr->forEach([&entities](Entity& entity) {
			if ((entity.hasComponent<ComponentT>() && ...))
				entities.push_back(std::ref(entity));
		});

		for (Entity& entity : entities) {
			act(entity);
		}
	}
}


template<typename... ComponentT>
void ECS::forEach(const std::function<void(const Entity&)>& act) const {

	static_assert((std::is_base_of_v<IComponent, ComponentT> && ...));

	if constexpr (sizeof...(ComponentT) == 0) {
		entity_mgr->forEach(act);
	}
	else {
		if ((!component_mgr->knowsComponent<ComponentT>() || ...))
			return;

		std::vector<std::reference_wrapper<const Entity>> entities;

		entity_mgr->forEach([&entities](const Entity& entity) {
			if ((entity.hasComponent<ComponentT>() && ...))
				entities.push_back(std::ref(entity));
		});

		for (const Entity& entity : entities) {
			act(entity);
		}
	}
}


template<typename ComponentT>
void ECS::forEach(const std::function<void(ComponentT&)>& act) {
	static_assert(std::is_base_of_v<IComponent, ComponentT>);

	if (!component_mgr->knowsComponent<ComponentT>())
		return;
	component_mgr->forEach<ComponentT>(act);
}


template<typename ComponentT>
void ECS::forEach(const std::function<void(const ComponentT&)>& act) const {
	static_assert(std::is_base_of_v<IComponent, ComponentT>);

	if (!component_mgr->knowsComponent<ComponentT>())
		return;
	component_mgr->forEach<ComponentT>(act);
}

} // namespace ecs