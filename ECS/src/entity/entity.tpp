#include "ecs.h"


//----------------------------------------------------------------------------------
// IEntity
//----------------------------------------------------------------------------------

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

	components[ComponentT::type_id] = component_mgr->CreateComponent<ComponentT>(std::forward<ArgsT>(args)...);
	components[ComponentT::type_id]->owner = handle;

	return static_cast<ComponentT* const>(components[ComponentT::type_id]);
}


template<typename ComponentT>
void IEntity::RemoveComponent() {

	auto it = components.find(ComponentT::type_id);

	component_mgr->DestroyComponent(it->second);
	components.erase(it);
}




//----------------------------------------------------------------------------------
// Entity
//----------------------------------------------------------------------------------


template<typename T>
template<typename... ArgsT>
void Entity<T>::Construct(ArgsT&&... args) {
	static_assert(std::is_base_of_v<Entity, T>, "Entity<T> - Invalid derived class.");
	Init(std::forward<ArgsT>(args)...);
}

template<typename T>
template<typename... ArgsT>
void Entity<T>::Init(ArgsT&&... args) {
	static_cast<T*>(this)->Init(std::forward<ArgsT>(args)...);
}