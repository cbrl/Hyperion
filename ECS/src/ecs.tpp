
namespace ecs {

//template<typename ComponentT, typename... ArgsT>
//ComponentT& ECS::addComponent(handle64 entity, ArgsT&& ... args) {
//
//}
//
//
//template<typename ComponentT>
//[[nodiscard]]
//ComponentT* ECS::getComponent(handle64 entity) {
//
//}
//
//
//template<typename ComponentT>
//[[nodiscard]]
//const ComponentT* ECS::getComponent(handle64 entity) const {
//
//}
//
//
//template<typename ComponentT>
//void ECS::removeComponent(handle64 entity) {
//
//}
//
//
//template<typename ComponentT>
//[[nodiscard]]
//bool ECS::hasComponent(handle64 entity) const {
//
//}
//
//
//template<typename ComponentT>
//[[nodiscard]]
//size_t ECS::count(handle64 entity) const {
//
//}


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

	// Iterator over all entities if no component types provided
	if constexpr (sizeof...(ComponentT) == 0) {
		entity_mgr->forEach(act);
	}
	else {
		// Return early if a type was provided that has never been used
		if ((!component_mgr->knowsComponent<ComponentT>() || ...))
			return;

		// Iterator over all entities that contain the provided component types
		entity_mgr->forEach([&act](Entity& entity) {
			if ((entity.hasComponent<ComponentT>() && ...))
				act(entity);
		});
	}
	
}


template<typename... ComponentT>
void ECS::forEach(const std::function<void(const Entity&)>& act) const {

	static_assert((std::is_base_of_v<IComponent, ComponentT> && ...));

	// Iterator over all entities if no component types provided
	if constexpr (sizeof...(ComponentT) == 0) {
		entity_mgr->forEach(act);
		return;
	}
	else {
		// Return early if a type was provided that has never been used
		if ((!component_mgr->knowsComponent<ComponentT>() || ...))
			return;

		// Iterator over all entities that contain the provided component types
		entity_mgr->forEach([&act](const Entity& entity) {
			if ((entity.hasComponent<ComponentT>() && ...))
				act(entity);
		});
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