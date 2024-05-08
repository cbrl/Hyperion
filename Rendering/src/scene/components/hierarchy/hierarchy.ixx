module;

#include <algorithm>
#include <functional>
#include <vector>

#include <memory/handle/handle.h>


export module rendering:components.hierarchy;

import ecs;


export class Hierarchy : public ecs::Component {
public:
	//----------------------------------------------------------------------------------
	// ParentChanged event
	//----------------------------------------------------------------------------------
	struct ParentChangedEvent {
		ParentChangedEvent(handle64 entity) : entity(entity) {}
		handle64 entity;
	};


	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Hierarchy() = default;
	Hierarchy(const Hierarchy&) = delete;
	Hierarchy(Hierarchy&&) noexcept = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~Hierarchy() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Hierarchy& operator=(const Hierarchy&) = delete;
	Hierarchy& operator=(Hierarchy&&) noexcept = default;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	handle64 getParent() const noexcept {
		return parent;
	}

	void setParent(ecs::ECS& ecs, handle64 parent) {
		if (this->parent != parent) {
			this->parent = parent;

			if (ecs.valid(parent)) {
				auto* parent_hierarchy = ecs.tryGet<Hierarchy>(parent);
				if (not parent_hierarchy) {
					parent_hierarchy = &ecs.add<Hierarchy>(parent);
				}

				parent_hierarchy->addChild(ecs, getOwner());
			}

			sendParentChangedEvent(ecs);
		}
	}

	void removeParent(ecs::ECS& ecs) {
		if (auto* parent_hierarchy = ecs.tryGet<Hierarchy>(parent)) {
			parent_hierarchy->removeChild(ecs, getOwner());
		}
	}

	[[nodiscard]]
	const std::vector<handle64>& getChildren() const noexcept {
		return children;
	}

	[[nodiscard]]
	bool hasChildren() const noexcept {
		return !children.empty();
	}

	[[nodiscard]]
	bool hasChild(handle64 child) const {
		return std::find(children.cbegin(), children.cend(), child) != children.cend();
	}

	// Make the specified entity a child of this entity
	void addChild(ecs::ECS& ecs, handle64 child) {
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

	// Remove the specified entity from this entity's children
	void removeChild(ecs::ECS& ecs, handle64 child) {
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

	// Remove all children from this entity
	void removeAllChildren(ecs::ECS& ecs) {
		forEachChild(ecs, [this, &ecs](handle64 child) {
			removeChild(ecs, child);
		});
	}

	// Apply an action to each child of this entity
	void forEachChild(ecs::ECS& ecs, const std::function<void(handle64)>& act) {
		bool prune = false;

		for (auto child : children) {
			if (ecs.valid(child))
				act(child);
			else
				prune = true;
		}

		if (prune) {
			pruneChildren(ecs);
		}
	}

	// Apply an action to each child of this entity
	void forEachChild(const ecs::ECS& ecs, const std::function<void(handle64)>& act) const {
		for (auto child : children) {
			if (ecs.valid(child))
				act(child);
		}
	}

	// Apply an action to each child of this entity, and each child of each child, etc...
	void forEachChildRecursive(ecs::ECS& ecs, const std::function<void(handle64)>& act) {
		bool prune = false;

		for (auto child : children) {
			if (ecs.valid(child)) {
				act(child);
				ecs.get<Hierarchy>(child).forEachChildRecursive(ecs, act);
			}
			else {
				prune = true;
			}
		}

		if (prune) {
			pruneChildren(ecs);
		}
	}

	// Apply an action to each child of this entity, and each child of each child, etc...
	void forEachChildRecursive(const ecs::ECS& ecs, const std::function<void(handle64)>& act) const {
		for (auto child : children) {
			if (ecs.valid(child)) {
				act(child);
				ecs.get<Hierarchy>(child).forEachChildRecursive(ecs, act);
			}
		}
	}

private:

	void sendParentChangedEvent(ecs::ECS& ecs) {
		ecs.enqueue<ParentChangedEvent>(getOwner());
	}

	void pruneChildren(ecs::ECS& ecs) {
		for (decltype(children)::iterator it = children.begin(); it != children.end();) {
			if (not ecs.valid(*it)) {
				it = children.erase(it);
			}
			else {
				++it;
			}
		}
	}

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	handle64 parent;
	std::vector<handle64> children;
};
