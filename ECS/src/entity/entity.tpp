#include "entity.h"
#include "component/component_mgr.h"


namespace ecs {

template<typename ComponentT, typename... ArgsT>
ComponentT& Entity::addComponent(ArgsT&&... args) {
	return component_mgr->addComponent<ComponentT>(this_handle, std::forward<ArgsT>(args)...);
}


template<typename ComponentT>
ComponentT& Entity::get() {
	return component_mgr->get<ComponentT>(this_handle);
}


template<typename ComponentT>
const ComponentT& Entity::get() const {
	return component_mgr->get<ComponentT>(this_handle);
}


template<typename ComponentT>
ComponentT* Entity::tryGet() {
	return component_mgr->tryGet<ComponentT>(this_handle);
}


template<typename ComponentT>
const ComponentT* Entity::tryGet() const {
	return component_mgr->tryGet<ComponentT>(this_handle);
}


template<typename ComponentT>
void Entity::removeComponent() {
	component_mgr->removeComponent<ComponentT>(this_handle);
}


template<typename ComponentT>
bool Entity::has() const {
	return component_mgr->has<ComponentT>(this_handle);
}

} // namespace ecs