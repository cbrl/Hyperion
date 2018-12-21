#include "entity.h"
#include "component/component_mgr.h"
#include "ecs.h"


Entity::~Entity() {
	if (hasParent() && parent_ptr.valid())
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


void Entity::setParent(EntityPtr parent) noexcept {
	if (parent_ptr != parent) {
		parent_ptr = parent;
		if (parent.valid()) {
			parent->addChild(this_ptr);
		}
		sendParentChangedEvent();
	}
}


void Entity::removeComponent(IComponent* component) {
	for (auto it = components.begin(); it != components.end(); ++it) {
		if (it->second == component) {
			components.erase(it);
			component_mgr->destroyComponent(component);
			return;
		}
	}
}


void Entity::addChild(EntityPtr child) {
	if (child.valid()
	    && child != this_ptr
	    && !hasChild(child)
		&& !child->hasChild(this_ptr)) {

		children.push_back(child);
		child->setParent(this_ptr);
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


bool Entity::hasChild(EntityPtr child) const {
	return std::find(children.cbegin(), children.cend(), child) != children.cend();
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
	name = "Entity (i:"
	       + ToStr(ptr.getHandle().index).value_or("-1"s)
	       + ", c:"
	       + ToStr(ptr.getHandle().counter).value_or("-1"s)
	       + ")";
}


void Entity::sendParentChangedEvent() {
	sendEvent<ParentChanged>(this_ptr);
}