#include "ecs.h"


//----------------------------------------------------------------------------------
// IEntity
//----------------------------------------------------------------------------------

template<typename ComponentT>
ComponentT* IEntity::getComponent() const {

	auto it = components.find(ComponentT::type_id);

	if (it == components.end())
		return nullptr;

	return static_cast<ComponentT* const>(it->second);
}


template<typename ComponentT, typename... ArgsT>
ComponentT* IEntity::addComponent(ArgsT&&... args) {

	auto it = components.find(ComponentT::type_id);

	// Nothing else to do if the component already exists
	if (it != components.end()) {
		return static_cast<ComponentT* const>(it->second);
	}

	components[ComponentT::type_id] = component_mgr->createComponent<ComponentT>(std::forward<ArgsT>(args)...);
	components[ComponentT::type_id]->owner = handle;

	return static_cast<ComponentT* const>(components[ComponentT::type_id]);
}


template<typename ComponentT>
void IEntity::removeComponent() {

	auto it = components.find(ComponentT::type_id);

	component_mgr->destroyComponent(it->second);
	components.erase(it);
}
