#include "entity.h"
#include "component/component_mgr.h"
#include "ecs.h"


IEntity::IEntity(EntityPtr this_ptr, gsl::not_null<ComponentMgr*> component_mgr)
	: active(true)
	, this_ptr(this_ptr)
	, component_mgr(component_mgr) {
}


IEntity::~IEntity() {
	for (auto& pair : components) {
		component_mgr->destroyComponent(pair.second);
		pair.second = nullptr;
	}
}


void IEntity::setActive(bool state) {
	active = state;
	for (auto& pair : components) {
		pair.second->setActive(state);
	}
}


void IEntity::addChild(EntityPtr child) {
	if (child != this_ptr
	    && child.valid()
	    && child->getParent() != this_ptr
		&& !child->hasChild(this_ptr)) {

		child->setParent(this_ptr);
		children.push_back(child);
	}
}


void IEntity::removeChild(EntityPtr child) {
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


void IEntity::removeAllChildren() {
	forEachChild([this](EntityPtr& child) {
		removeChild(child);
	});
}


bool IEntity::hasChild(EntityPtr child) const {
	return std::find(children.cbegin(), children.cend(), child) != children.cend();
}