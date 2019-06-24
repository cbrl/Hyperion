#include "entity.h"
#include "component/component_mgr.h"
#include "ecs.h"
#include <functional>


namespace ecs {

Entity::Entity(Entity&& other) noexcept
	: EventSender(std::move(other)) {
	name          = std::move(other.name);
	this_ptr      = std::exchange(other.this_ptr, EntityPtr{});
	active        = std::move(other.active);
	component_mgr = std::exchange(other.component_mgr, nullptr);
	parent_ptr    = std::exchange(other.parent_ptr, EntityPtr{});
	children      = std::move(other.children);
}


Entity& Entity::operator=(Entity&& other) noexcept {
	EventSender::operator=(std::move(other));
	name          = std::move(other.name);
	this_ptr      = std::exchange(other.this_ptr, EntityPtr{});
	active        = std::move(other.active);
	component_mgr = std::exchange(other.component_mgr, nullptr);
	parent_ptr    = std::exchange(other.parent_ptr, EntityPtr{});
	children      = std::move(other.children);
	return *this;
}


std::string& Entity::getName() noexcept {
	return name;
}


const std::string& Entity::getName() const noexcept {
	return name;
}


void Entity::setName(std::string new_name) noexcept {
	name = std::move(new_name);
}


EntityPtr Entity::getPtr() const noexcept {
	return this_ptr;
}


void Entity::setActive(bool state) noexcept {
	active = state;
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


void Entity::removeParent() {
	if (parent_ptr) {
		parent_ptr->removeChild(this_ptr);
	}
}


void Entity::setParent(EntityPtr parent) {
	if (parent_ptr != parent) {
		parent_ptr = parent;
		if (parent.valid()) {
			parent->addChild(this_ptr);
		}
		sendParentChangedEvent();
	}
}


void Entity::removeComponent(IComponent& component) {
	component_mgr->destroyComponent(this_ptr.getHandle(), component);
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
	forEachChild([this](Entity& child) {
		removeChild(child.getPtr());
	});
}


bool Entity::hasChild(EntityPtr child) const {
	return std::find(children.cbegin(), children.cend(), child) != children.cend();
}


bool Entity::hasChildren() const noexcept {
	return !children.empty();
}


void Entity::forEachChild(const std::function<void(Entity&)>& act) {
	for (auto child : children) {
		if (child.valid())
			act(*child);
	}
}


void Entity::forEachChild(const std::function<void(const Entity&)>& act) const {
	for (auto child : children) {
		if (child.valid())
			act(*child);
	}
}


void Entity::forEachChildRecursive(const std::function<void(Entity&)>& act) {
	for (auto child : children) {
		if (child.valid()) {
			auto& child_entity = *child;
			act(child_entity);
			child_entity.forEachChildRecursive(act);
		}
	}
}


void Entity::forEachChildRecursive(const std::function<void(const Entity&)>& act) const {
	for (auto child : children) {
		if (child.valid()) {
			const auto& child_entity = *child;
			act(child_entity);
			child_entity.forEachChildRecursive(act);
		}
	}
}


void Entity::setComponentMgr(gsl::not_null<ComponentMgr*> mgr) {
	component_mgr = mgr;
}


void Entity::setPointer(EntityPtr ptr) noexcept {
	this_ptr = ptr;
	forEachChild([ptr](Entity& child) {
		child.setParent(ptr);
	});
	name = "Entity (i:"
	       + ToStr(ptr.getHandle().index).value_or("-1"s)
	       + ", c:"
	       + ToStr(ptr.getHandle().counter).value_or("-1"s)
	       + ")";
}


void Entity::sendParentChangedEvent() {
	sendEvent<ParentChangedEvent>(this_ptr);
}

} // namespace ecs