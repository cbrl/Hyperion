
template<typename ComponentT>
ComponentT* IEntity::GetComponent() const {

	auto it = components.find(ComponentT::type_id);

	if (it == components.end())
		return nullptr;

	return static_cast<ComponentT*>(it->second);
}


template<typename ComponentT, typename... ArgsT>
void IEntity::AddComponent(ArgsT&&... args) {

	auto it = components.find(ComponentT::type_id);

	// Nothing to do if the component already exists
	if (it != components.end()) return;

	if (auto tmp = component_mgr.lock()) {
		components[ComponentT::type_id] = tmp->CreateComponent<ComponentT>(std::forward(args)...);
		components[ComponentT::type_id]->owner = handle;
	}
}


template<typename ComponentT>
void IEntity::RemoveComponent() {

	auto it = components.find(ComponentT::type_id);

	if (it != components.end()) {
		component_mgr.lock()->DestroyComponent(it->second);
		components.erase(it);
	}
}