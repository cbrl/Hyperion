
template<typename ComponentT>
ComponentT* IEntity::GetComponent() const {

	auto it = components.find(ComponentT::type_idx);

	if (it == components.end())
		return nullptr;

	return static_cast<ComponentT*>(it->second);
}


template<typename ComponentT, typename... ArgsT>
void IEntity::AddComponent(ArgsT&&... args) {

	auto it = components.find(ComponentT::type_idx);

	// Nothing to do if the component already exists
	if (it != components.end()) return;

	components[ComponentT::type_idx] = component_mgr.lock()->CreateComponent<ComponentT>(std::forward(args)...);

	components[ComponentT::type_idx]->owner = handle;
}


template<typename ComponentT>
void IEntity::RemoveComponent() {

	auto it = components.find(ComponentT::type_idx);

	if (it != components.end()) {
		component_mgr.lock()->DestroyComponent(it->second);
		components.erase(it);
	}
}