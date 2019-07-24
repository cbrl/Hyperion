#pragma once

#include "component/component.h"


namespace ecs {
	class ECS;
}

class Hierarchy : public ecs::Component<Hierarchy>, public ecs::EventSender {
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
	handle64 getParent() const noexcept;

	void setParent(ecs::ECS& ecs, handle64 parent);

	void removeParent(ecs::ECS& ecs);

	[[nodiscard]]
	const std::vector<handle64>& getChildren() const noexcept;

	[[nodiscard]]
	bool hasChildren() const noexcept;

	[[nodiscard]]
	bool hasChild(handle64 child) const;

	// Make the specified entity a child of this entity
	void addChild(ecs::ECS& ecs, handle64 child);

	// Remove the specified entity from this entity's children
	void removeChild(ecs::ECS& ecs, handle64 child);

	// Remove all children from this entity
	void removeAllChildren(ecs::ECS& ecs);

	// Apply an action to each child of this entity
	void forEachChild(ecs::ECS& ecs, const std::function<void(handle64)>& act);

	// Apply an action to each child of this entity
	void forEachChild(const ecs::ECS& ecs, const std::function<void(handle64)>& act) const;

	// Apply an action to each child of this entity, and each child of each child, etc...
	void forEachChildRecursive(ecs::ECS& ecs, const std::function<void(handle64)>& act);

	// Apply an action to each child of this entity, and each child of each child, etc...
	void forEachChildRecursive(const ecs::ECS& ecs, const std::function<void(handle64)>& act) const;

private:

	void sendParentChangedEvent();

	void pruneChildren(ecs::ECS& ecs);

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	handle64 parent;
	std::vector<handle64> children;
};