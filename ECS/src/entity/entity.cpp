#include "entity.h"
#include "component/component_mgr.h"
#include "ecs.h"


Entity::Entity(EntityPtr this_ptr, gsl::not_null<ComponentMgr*> component_mgr)
	: active(true)
	, this_ptr(this_ptr)
	, component_mgr(component_mgr) {
}


Entity::~Entity() {

	removeAllChildren();

	for (auto& pair : components) {
		component_mgr->destroyComponent(pair.second);
		pair.second = nullptr;
	}
}


void Entity::setActive(bool state) {
	active = state;
	for (auto& pair : components) {
		pair.second->setActive(state);
	}
}


void Entity::addChild(EntityPtr child) {
	if (child != this_ptr
	    && child.valid()
	    && child->getParent() != this_ptr
		&& !child->hasChild(this_ptr)) {

		child->setParent(this_ptr);
		children.push_back(child);
	}
}


void Entity::removeChild(EntityPtr child) {
	if (child == this_ptr
		|| !child.valid()
		|| child->getParent() != this_ptr) {
		return;
	}

	const auto it = std::find(children.cbegin(), children.cend(), child);
	if (it != children.cend()) {
		child->setParent(EntityPtr{});
		children.erase(it);
	}
}


void Entity::removeAllChildren() {
	forEachChild([this](EntityPtr& child) {
		removeChild(child);
	});
}


bool Entity::hasChild(EntityPtr child) const {
	return std::find(children.cbegin(), children.cend(), child) != children.cend();
}