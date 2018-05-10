#include "ecs\ecs.h"

template<typename ComponentT>
ComponentT* const IEntity::GetComponent() const {

	auto it = components.find(ComponentT::type_id);

	if (it == components.end())
		return nullptr;

	return static_cast<ComponentT* const>(it->second);
}


template<typename ComponentT, typename... ArgsT>
ComponentT* const IEntity::AddComponent(ArgsT&&... args) {

	auto it = components.find(ComponentT::type_id);

	// Nothing else to do if the component already exists
	if (it != components.end()) {
		return static_cast<ComponentT* const>(it->second);
	}

	components[ComponentT::type_id] = ECS::Get()->GetComponentMgr()->CreateComponent<ComponentT>(std::forward<ArgsT>(args)...);
	components[ComponentT::type_id]->owner = handle;

	return static_cast<ComponentT* const>(components[ComponentT::type_id]);
}


template<typename ComponentT>
void IEntity::RemoveComponent() {

	auto it = components.find(ComponentT::type_id);

	ECS::Get()->GetComponentMgr()->DestroyComponent(it->second);
	components.erase(it);
}