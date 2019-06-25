#include "entity.h"
#include "component/component_mgr.h"


namespace ecs {

template<typename ComponentT, typename... ArgsT>
ComponentT& Entity::addComponent(ArgsT&&... args) {
	return component_mgr->addComponent<ComponentT>(this_handle, std::forward<ArgsT>(args)...);
}


template<typename ComponentT>
ComponentT& Entity::getComponent() {
	return component_mgr->getComponent<ComponentT>(this_handle);
}


template<typename ComponentT>
const ComponentT& Entity::getComponent() const {
	return component_mgr->getComponent<ComponentT>(this_handle);
}


template<typename ComponentT>
ComponentT* Entity::tryGetComponent() {
	return component_mgr->tryGetComponent<ComponentT>(this_handle);
}


template<typename ComponentT>
const ComponentT* Entity::tryGetComponent() const {
	return component_mgr->tryGetComponent<ComponentT>(this_handle);
}


template<typename ComponentT>
void Entity::removeComponent() {
	component_mgr->removeComponent<ComponentT>(this_handle);
}


template<typename ComponentT>
bool Entity::hasComponent() const {
	return component_mgr->hasComponent<ComponentT>(this_handle);
}

} // namespace ecs