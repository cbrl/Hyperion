#include "entity.h"
#include "component/component_mgr.h"

namespace ecs {

template<typename ComponentT, typename... ArgsT>
ComponentT& Entity::addComponent(ArgsT&&... args) {
	ComponentT& component = component_mgr->createComponent<ComponentT>(this_ptr.getHandle(), std::forward<ArgsT>(args)...);
	component.setOwner(this_ptr);

	return component;
}


template<typename ComponentT>
ComponentT& Entity::getComponent() {
	return component_mgr->getComponent<ComponentT>(this_ptr.getHandle());
}


template<typename ComponentT>
const ComponentT& Entity::getComponent() const {
	return component_mgr->getComponent<ComponentT>(this_ptr.getHandle());
}


template<typename ComponentT>
ComponentT* Entity::tryGetComponent() {
	return component_mgr->tryGetComponent<ComponentT>(this_ptr.getHandle());
}


template<typename ComponentT>
const ComponentT* Entity::tryGetComponent() const {
	return component_mgr->tryGetComponent<ComponentT>(this_ptr.getHandle());
}


template<typename ComponentT>
void Entity::removeComponent(ComponentT& component) {
	component_mgr->destroyComponent<ComponentT>(this_ptr.getHandle());
}


template<typename ComponentT>
bool Entity::hasComponent() const {
	return component_mgr->hasComponent<ComponentT>(this_ptr.getHandle());
}

} // namespace ecs