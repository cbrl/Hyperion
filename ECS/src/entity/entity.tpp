#include "entity.h"


template<typename ComponentT, typename... ArgsT>
ComponentT* IEntity::addComponent(ArgsT&&... args) {

	ComponentT* component = component_mgr->createComponent<ComponentT>(std::forward<ArgsT>(args)...);
	component->setOwner(handle);

	components.emplace(ComponentT::type_id, component);

	return static_cast<ComponentT* const>(component);
}


template<typename ComponentT>
void IEntity::removeComponent(ComponentT* component) {

	const auto range = components.equal_range(ComponentT::type_id);

	for (auto it = range.first; it != range.second; ++it) {
		if (it->second == component) {
			components.erase(it);
			break;
		}
	}
}


template<typename ComponentT>
void IEntity::removeAll() {

	const auto range = components.equal_range(ComponentT::type_id);

	components.erase(range.first, range.second);
}


template<typename ComponentT>
bool IEntity::hasComponent() const {
	return components.find(ComponentT::type_id) != components.end();
}


template<typename ComponentT>
size_t IEntity::countOf() const {
	return components.count(ComponentT::type_id);
}


template<typename ComponentT>
ComponentT* IEntity::getComponent() {

	auto it = components.find(ComponentT::type_id);

	if (it == components.end())
		return nullptr;

	return static_cast<ComponentT* const>(it->second);
}


template<typename ComponentT>
std::vector<ComponentT*> IEntity::getAll() {
	
	std::vector<ComponentT*> component_vec;

	const auto range = components.equal_range(ComponentT::type_id);

	std::for_each(range.first, range.second, [&](auto& pair) {
		component_vec.push_back(static_cast<ComponentT*>(pair.second));
	});

	return component_vec;
}