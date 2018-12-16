#include "entity.h"
#include "component/component_mgr.h"
#include "ecs.h"


Entity::~Entity() {
	if (hasParent())
		parent_ptr->removeChild(getPtr());

	removeAllChildren();

	for (auto& pair : components) {
		component_mgr->destroyComponent(pair.second);
		pair.second = nullptr;
	}
}


EntityPtr Entity::getPtr() const noexcept {
	return this_ptr;
}


void Entity::setActive(bool state) noexcept {
	active = state;
	for (auto& pair : components) {
		pair.second->setActive(state);
	}
}


bool Entity::isActive() const noexcept {
	return active;
}


EntityPtr Entity::getParent() const noexcept {
	return parent_ptr;
}


bool Entity::hasParent() const noexcept {
	return bool(parent_ptr);
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
	if (!child.valid()
	    || child == this_ptr
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


bool Entity::hasChildren() const noexcept {
	return !children.empty();
}


void Entity::setComponentMgr(gsl::not_null<ComponentMgr*> mgr) {
	component_mgr = mgr;
}


void Entity::setPointer(EntityPtr ptr) noexcept {
	this_ptr = ptr;
	forEachChild([ptr](EntityPtr& child) {
		child->setParent(ptr);
	});
	for (auto& pair : components) {
		pair.second->setOwner(ptr);
	}
}


void Entity::setParent(EntityPtr parent) noexcept {
	parent_ptr = parent;
}


bool Entity::hasChild(EntityPtr child) const {
	return std::find(children.cbegin(), children.cend(), child) != children.cend();
}