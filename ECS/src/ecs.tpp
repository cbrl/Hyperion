#include "entity/entity_mgr.h"
#include "event/event_mgr.h"
#include "system/system_mgr.h"
#include "component/component_mgr.h"


namespace ecs {


//----------------------------------------------------------------------------------
// Components
//----------------------------------------------------------------------------------

template<typename ComponentT, typename... ArgsT, typename>
ComponentT& ECS::add(handle64 entity, ArgsT&&... args) {
	return component_mgr->add<ComponentT>(entity, std::forward<ArgsT>(args)...);
}


template<typename ComponentT, typename>
void ECS::remove(handle64 entity) {
	component_mgr->remove<ComponentT>(entity);
}


template<typename ComponentT>
bool ECS::has(handle64 entity) const {
	return component_mgr->has<ComponentT>(entity);
}


template<typename ComponentT, typename>
ComponentT& ECS::get(handle64 entity) {
	return component_mgr->get<ComponentT>(entity);
}


template<typename ComponentT, typename>
const ComponentT& ECS::get(handle64 entity) const {
	return component_mgr->get<ComponentT>(entity);
}


template<typename ComponentT, typename>
ComponentT* ECS::tryGet(handle64 entity) {
	return component_mgr->tryGet<ComponentT>(entity);
}


template<typename ComponentT, typename>
const ComponentT* ECS::tryGet(handle64 entity) const {
	return component_mgr->tryGet<ComponentT>(entity);
}



//----------------------------------------------------------------------------------
// Systems
//----------------------------------------------------------------------------------

template<typename SystemT, typename... ArgsT, typename>
SystemT& ECS::add(ArgsT&&... args) {
	return system_mgr->add<SystemT>(std::forward<ArgsT>(args)...);
}


template<typename SystemT, typename>
void ECS::remove() {
	system_mgr->remove<SystemT>();
}


template<typename SystemT, typename>
SystemT& ECS::get() {
	return system_mgr->get<SystemT>();
}


template<typename SystemT, typename>
const SystemT& ECS::get() const {
	return system_mgr->get<SystemT>();
}


template<typename SystemT, typename>
SystemT* ECS::tryGet() {
	return system_mgr->tryGet<SystemT>();
}


template<typename SystemT, typename>
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
void ECS::enqueue(ArgsT&&... args) {
	event_mgr->enqueue<EventT>(std::forward<ArgsT>(args)...);
}

template<typename EventT, typename... ArgsT>
void ECS::send(ArgsT&& ... args) {
	event_mgr->send<EventT>(std::forward<ArgsT>(args)...);
}

template<typename EventT>
EventDispatcher<EventT>& ECS::getDispatcher() {
	return event_mgr->getDispatcher<EventT>();
}

template<typename EventT>
void ECS::removeCallback(const std::function<void(const EventT&)>& callback) {
	event_mgr->removeCallback(callback);
}



//----------------------------------------------------------------------------------
// Iteration
//----------------------------------------------------------------------------------

template<typename... ComponentT>
requires (std::derived_from<ComponentT, IComponent> && ...)
void ECS::forEach(std::function<void(handle64)> act) {
	if constexpr (sizeof...(ComponentT) == 0) {
		entity_mgr->forEach(act);
	}
	else {
		// Return early if a type was provided that has never been used
		if ((!component_mgr->knowsComponent<ComponentT>() || ...))
			return;

		// Iterate over all entities that contain the provided component types
		entity_mgr->forEach([this, &act](handle64 entity) {
			if ((has<ComponentT>(entity) && ...))
				act(entity);
		});
	}
}


template<typename... ComponentT>
requires (std::derived_from<ComponentT, IComponent> && ...)
void ECS::forEach(std::function<void(handle64)> act) const {
	if constexpr (sizeof...(ComponentT) == 0) {
		entity_mgr->forEach(act);
	}
	else {
		// Return early if a type was provided that has never been used
		if ((!component_mgr->knowsComponent<ComponentT>() || ...))
			return;

		// Iterate over all entities that contain the provided component types
		entity_mgr->forEach([this, &act](handle64 entity) {
			if ((has<ComponentT>(entity) && ...))
				act(entity);
		});
	}
}


template<typename ComponentT>
requires std::derived_from<ComponentT, IComponent>
void ECS::forEach(std::function<void(ComponentT&)> act) {
	if (!component_mgr->knowsComponent<ComponentT>())
		return;
	component_mgr->forEach<ComponentT>(std::ref(act));
}


template<typename ComponentT>
requires std::derived_from<ComponentT, IComponent>
void ECS::forEach(std::function<void(const ComponentT&)> act) const {
	if (!component_mgr->knowsComponent<ComponentT>())
		return;
	component_mgr->forEach<ComponentT>(std::ref(act));
}



//----------------------------------------------------------------------------------
// Count
//----------------------------------------------------------------------------------

template<typename T>
size_t ECS::count() const {
	if constexpr (std::is_same_v<handle64, T>)
		return entity_mgr->count();
	else
		return component_mgr->count<T>();
}


} // namespace ecs