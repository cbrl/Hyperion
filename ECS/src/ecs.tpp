#include "entity/entity_mgr.h"
#include "event/event_mgr.h"
#include "system/system_mgr.h"
#include "component/component_mgr.h"


namespace ecs {


//----------------------------------------------------------------------------------
// Components
//----------------------------------------------------------------------------------

template<typename ComponentT, typename... ArgsT>
ComponentT& ECS::addComponent(handle64 entity, ArgsT&&... args) {
	return component_mgr->addComponent<ComponentT>(entity, std::forward<ArgsT>(args)...);
}


template<typename ComponentT>
void ECS::removeComponent(handle64 entity) {
	component_mgr->removeComponent<ComponentT>(entity);
}


template<typename ComponentT>
bool ECS::has(handle64 entity) const {
	return component_mgr->has<ComponentT>(entity);
}


template<typename ComponentT>
ComponentT& ECS::get(handle64 entity) {
	return component_mgr->get<ComponentT>(entity);
}


template<typename ComponentT>
const ComponentT& ECS::get(handle64 entity) const {
	return component_mgr->get<ComponentT>(entity);
}


template<typename ComponentT>
ComponentT* ECS::tryGet(handle64 entity) {
	return component_mgr->tryGet<ComponentT>(entity);
}


template<typename ComponentT>
const ComponentT* ECS::tryGet(handle64 entity) const {
	return component_mgr->tryGet<ComponentT>(entity);
}



//----------------------------------------------------------------------------------
// Systems
//----------------------------------------------------------------------------------

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
SystemT& ECS::get() {
	return system_mgr->get<SystemT>();
}


template<typename SystemT>
const SystemT& ECS::get() const {
	return system_mgr->get<SystemT>();
}


template<typename SystemT>
SystemT* ECS::tryGet() {
	return system_mgr->tryGet<SystemT>();
}


template<typename SystemT>
const SystemT* ECS::tryGet() const {
	return system_mgr->tryGet<SystemT>();
}


template<typename SystemT>
void ECS::setSystemPriority(u32 priority) {
	system_mgr->setSystemPriority<SystemT>(priority);
}



//----------------------------------------------------------------------------------
// Events
//----------------------------------------------------------------------------------

template<typename EventT, typename... ArgsT>
void ECS::sendEvent(ArgsT&&... args) {
	event_mgr->send<EventT>(std::forward<ArgsT>(args)...);
}



//----------------------------------------------------------------------------------
// Iteration
//----------------------------------------------------------------------------------

template<typename... ComponentT>
void ECS::forEach(std::function<void(Entity&)> act) {

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
			if ((entity.has<ComponentT>() && ...))
				act(entity);
		});
	}
}


template<typename... ComponentT>
void ECS::forEach(std::function<void(const Entity&)> act) const {

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
			if ((entity.has<ComponentT>() && ...))
				act(entity);
		});
	}
}


template<typename ComponentT>
void ECS::forEach(std::function<void(ComponentT&)> act) {
	static_assert(std::is_base_of_v<IComponent, ComponentT>);

	if (!component_mgr->knowsComponent<ComponentT>())
		return;
	component_mgr->forEach<ComponentT>(std::ref(act));
}


template<typename ComponentT>
void ECS::forEach(std::function<void(const ComponentT&)> act) const {
	static_assert(std::is_base_of_v<IComponent, ComponentT>);

	if (!component_mgr->knowsComponent<ComponentT>())
		return;
	component_mgr->forEach<ComponentT>(std::ref(act));
}



//----------------------------------------------------------------------------------
// Count
//----------------------------------------------------------------------------------

template<typename T>
size_t ECS::count() const {
	if constexpr (std::is_same_v<Entity, T>)
		return entity_mgr->count();
	else
		return component_mgr->count<T>();
}


} // namespace ecs