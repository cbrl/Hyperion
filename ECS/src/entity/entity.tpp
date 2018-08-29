#include "entity.h"
#include "component/component_mgr.h"


template<typename ComponentT, typename... ArgsT>
ComponentT* IEntity::addComponent(ArgsT&&... args) {

	ComponentT* component = component_mgr->createComponent<ComponentT>(std::forward<ArgsT>(args)...);
	component->setOwner(this_ptr);

	components.emplace(ComponentT::index, component);

	return static_cast<ComponentT* const>(component);
}


template<typename ComponentT>
ComponentT* IEntity::getComponent() {

	auto it = components.find(ComponentT::index);

	if (it == components.end())
		return nullptr;

	return static_cast<ComponentT* const>(it->second);
}


template<typename ComponentT>
std::vector<ComponentT*> IEntity::getAll() {

	std::vector<ComponentT*> component_vec;

	const auto range = components.equal_range(ComponentT::index);

	std::for_each(range.first, range.second, [&](auto& pair) {
		component_vec.push_back(static_cast<ComponentT*>(pair.second));
	});

	return component_vec;
}


template<typename ComponentT>
void IEntity::removeComponent(ComponentT* component) {

	const auto range = components.equal_range(ComponentT::index);

	for (auto it = range.first; it != range.second; ++it) {
		if (it->second == component) {
			components.erase(it);
			break;
		}
	}
}


template<typename ComponentT>
void IEntity::removeAll() {

	const auto range = components.equal_range(ComponentT::index);
	components.erase(range.first, range.second);
}


template<typename ComponentT>
bool IEntity::hasComponent() const {
	return components.find(ComponentT::index) != components.end();
}


template<typename ComponentT>
size_t IEntity::countOf() const {
	return components.count(ComponentT::index);
}


template<typename ActionT>
void IEntity::forEachChild(const ActionT& act) {
	children.erase(std::remove_if(children.begin(), children.end(),
	                              [](EntityPtr& child) { return !child.valid(); }),
	               children.end());

	for (auto child : children) {
		act(child);
	}
}


template<typename ActionT>
void IEntity::forEachChildRecursive(const ActionT& act) {
	children.erase(std::remove_if(children.begin(), children.end(),
	                              [](EntityPtr& child) { return !child.valid(); }),
	               children.end());

	for (auto child : children) {
		child->forEachChildRecursive(act);
		act(child);
	}
}