#include "hierarchy.h"
#include "ecs.h"


handle64 Hierarchy::getParent() const noexcept {
	return parent;
}


void Hierarchy::removeParent(ecs::ECS& ecs) {
	if (auto* parent_hierarchy = ecs.tryGet<Hierarchy>(parent)) {
		parent_hierarchy->removeChild(ecs, getOwner());
	}
}


void Hierarchy::setParent(ecs::ECS& ecs, handle64 parent) {
	if (this->parent != parent) {
		this->parent = parent;

		if (ecs.valid(parent)) {
			auto* parent_hierarchy = ecs.tryGet<Hierarchy>(parent);
			if (not parent_hierarchy) {
				parent_hierarchy = &ecs.add<Hierarchy>(parent);
			}

			parent_hierarchy->addChild(ecs, getOwner());
		}

		sendParentChangedEvent();
	}
}


const std::vector<handle64>& Hierarchy::getChildren() const noexcept {
	return children;
}


bool Hierarchy::hasChildren() const noexcept {
	return !children.empty();
}


bool Hierarchy::hasChild(handle64 child) const {
	return std::find(children.cbegin(), children.cend(), child) != children.cend();
}


void Hierarchy::addChild(ecs::ECS& ecs, handle64 child) {
	if (ecs.valid(child)
		&& child != getOwner()
		&& !hasChild(child)) {

		auto* child_hierarchy = ecs.tryGet<Hierarchy>(child);
		if (child_hierarchy) {
			if (child_hierarchy->hasChild(getOwner()))
				return;
		}
		else {
			child_hierarchy = &ecs.add<Hierarchy>(child);
		}

		children.push_back(child);
		child_hierarchy->setParent(ecs, getOwner());
	}
}


void Hierarchy::removeChild(ecs::ECS& ecs, handle64 child) {
	if (!ecs.valid(child) || child == getOwner())
		return;

	auto* child_hierarchy = ecs.tryGet<Hierarchy>(child);
	if (!child_hierarchy || child_hierarchy->getParent() != getOwner())
		return;

	const auto it = std::find(children.cbegin(), children.cend(), child);
	if (it != children.cend()) {
		child_hierarchy->setParent(ecs, handle64{});
		children.erase(it);
	}
}


void Hierarchy::removeAllChildren(ecs::ECS& ecs) {
	forEachChild(ecs, [this, &ecs](handle64 child) {
		removeChild(ecs, child);
	});
}


void Hierarchy::forEachChild(ecs::ECS& ecs, const std::function<void(handle64)>& act) {
	for (auto child : children) {
		if (ecs.valid(child))
			act(child);
		else
			removeChild(ecs, child);
	}
}


void Hierarchy::forEachChild(const ecs::ECS& ecs, const std::function<void(handle64)>& act) const {
	for (auto child : children) {
		if (ecs.valid(child))
			act(child);
	}
}


void Hierarchy::forEachChildRecursive(ecs::ECS& ecs, const std::function<void(handle64)>& act) {
	for (auto child : children) {
		if (ecs.valid(child)) {
			act(child);
			ecs.get<Hierarchy>(child).forEachChildRecursive(ecs, act);
		}
		else {
			removeChild(ecs, child);
		}
	}
}


void Hierarchy::forEachChildRecursive(const ecs::ECS& ecs, const std::function<void(handle64)>& act) const {
	for (auto child : children) {
		if (ecs.valid(child)) {
			act(child);
			ecs.get<Hierarchy>(child).forEachChildRecursive(ecs, act);
		}
	}
}


void Hierarchy::sendParentChangedEvent() {
	enqueue<ParentChangedEvent>(getOwner());
}