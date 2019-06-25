#include "entity.h"
#include "entity/entity_mgr.h"
#include "component/component_mgr.h"
#include <functional>


namespace ecs {

Entity::Entity(Entity&& other) noexcept = default;


Entity& Entity::operator=(Entity&& other) noexcept = default;


std::string& Entity::getName() noexcept {
	return name;
}


const std::string& Entity::getName() const noexcept {
	return name;
}


void Entity::setName(std::string new_name) noexcept {
	name = std::move(new_name);
}


handle64 Entity::getHandle() const noexcept {
	return this_handle;
}


handle64 Entity::getParent() const noexcept {
	return parent_handle;
}


bool Entity::hasParent() const noexcept {
	return entity_mgr->valid(parent_handle);
}


void Entity::removeParent() {
	if (entity_mgr->valid(parent_handle)) {
		entity_mgr->getEntity(parent_handle).removeChild(this_handle);
	}
}


void Entity::setParent(handle64 parent) {
	if (parent_handle != parent) {
		parent_handle = parent;
		if (entity_mgr->valid(parent)) {
			entity_mgr->getEntity(parent).addChild(this_handle);
		}
		sendParentChangedEvent();
	}
}


void Entity::removeComponent(IComponent& component) {
	component_mgr->removeComponent(this_handle, component);
}


void Entity::addChild(handle64 child) {
	if (entity_mgr->valid(child)
	    && child != this_handle
	    && !hasChild(child)
		&& !entity_mgr->getEntity(child).hasChild(this_handle)) {

		children.push_back(child);
		entity_mgr->getEntity(child).setParent(this_handle);
	}
}


void Entity::removeChild(handle64 child) {
	if (!entity_mgr->valid(child)
	    || child == this_handle
		|| entity_mgr->getEntity(child).getParent() != this_handle) {
		return;
	}

	const auto it = std::find(children.cbegin(), children.cend(), child);
	if (it != children.cend()) {
		entity_mgr->getEntity(child).setParent(handle64{});
		children.erase(it);
	}
}


void Entity::removeAllChildren() {
	forEachChild([this](Entity& child) {
		removeChild(child.getHandle());
	});
}


bool Entity::hasChild(handle64 child) const {
	return std::find(children.cbegin(), children.cend(), child) != children.cend();
}


bool Entity::hasChildren() const noexcept {
	return !children.empty();
}


void Entity::forEachChild(const std::function<void(Entity&)>& act) {
	for (auto child : children) {
		if (entity_mgr->valid(child))
			act(entity_mgr->getEntity(child));
	}
}


void Entity::forEachChild(const std::function<void(const Entity&)>& act) const {
	for (auto child : children) {
		if (entity_mgr->valid(child))
			act(entity_mgr->getEntity(child));
	}
}


void Entity::forEachChildRecursive(const std::function<void(Entity&)>& act) {
	for (auto child : children) {
		if (entity_mgr->valid(child)) {
			auto& child_entity = entity_mgr->getEntity(child);
			act(child_entity);
			child_entity.forEachChildRecursive(act);
		}
	}
}


void Entity::forEachChildRecursive(const std::function<void(const Entity&)>& act) const {
	for (auto child : children) {
		if (entity_mgr->valid(child)) {
			const auto& child_entity = entity_mgr->getEntity(child);
			act(child_entity);
			child_entity.forEachChildRecursive(act);
		}
	}
}


void Entity::setEntityMgr(gsl::not_null<EntityMgr*> mgr) noexcept {
	entity_mgr = mgr;
}


void Entity::setComponentMgr(gsl::not_null<ComponentMgr*> mgr) noexcept {
	component_mgr = mgr;
}


void Entity::setHandle(handle64 handle) noexcept {
	this_handle = handle;
	forEachChild([handle](Entity& child) {
		child.setParent(handle);
	});
	name = "Entity (i:"
	       + ToStr(handle.index).value_or("-1"s)
	       + ", c:"
	       + ToStr(handle.counter).value_or("-1"s)
	       + ")";
}


void Entity::sendParentChangedEvent() {
	sendEvent<ParentChangedEvent>(this_handle);
}

} // namespace ecs