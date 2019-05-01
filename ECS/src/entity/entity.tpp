#include "entity.h"
#include "component/component_mgr.h"

namespace ecs {

template<typename ComponentT, typename... ArgsT>
ComponentT& Entity::addComponent(ArgsT&&... args) {

	ComponentT& component = component_mgr->createComponent<ComponentT>(std::forward<ArgsT>(args)...);
	component.setOwner(this_ptr);

	components.emplace(ComponentT::index, component);

	return component;
}


template<typename ComponentT>
ComponentT* Entity::getComponent() {

	auto it = components.find(ComponentT::index);

	if (it == components.end())
		return nullptr;

	return static_cast<ComponentT*>(&it->second.get());
}


template<typename ComponentT>
std::vector<std::reference_wrapper<ComponentT>> Entity::getAll() {

	std::vector<std::reference_wrapper<ComponentT>> component_vec;

	const auto range = components.equal_range(ComponentT::index);

	std::for_each(range.first, range.second, [&](auto& pair) {
		component_vec.push_back(static_cast<ComponentT&>(pair.second.get()));
	});

	return component_vec;
}


template<typename ComponentT>
void Entity::removeComponent(ComponentT& component) {

	const auto range = components.equal_range(ComponentT::index);

	for (auto it = range.first; it != range.second; ++it) {
		if (&it->second.get() == &component) {
			components.erase(it);
			component_mgr->destroyComponent(component);
			return;
		}
	}
}


template<typename ComponentT>
void Entity::removeAll() {

	const auto range = components.equal_range(ComponentT::index);

	for (auto it = range.first; it != range.second; ++it) {
		component_mgr->destroyComponent(it->second);
	}
	components.erase(range.first, range.second);
}


template<typename ComponentT>
bool Entity::hasComponent() const {
	return components.find(ComponentT::index) != components.end();
}


template<typename ComponentT>
size_t Entity::countOf() const {
	return components.count(ComponentT::index);
}


template<typename ActionT>
void Entity::forEachChild(ActionT&& act) {
	for (auto child : children) {
		if (child.valid()) act(child);
	}
}


template<typename ActionT>
void Entity::forEachChildRecursive(ActionT&& act) {
	for (auto child : children) {
		if (child.valid()) {
			act(child);
			child->forEachChildRecursive(act);
		}
	}
}

} // namespace ecs