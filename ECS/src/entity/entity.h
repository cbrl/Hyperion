#pragma once

#include "event/event.h"
#include "event/event_participator.h"
#include "memory/handle/handle.h"

namespace ecs {

class EntityMgr;
class ComponentMgr;
class IComponent;

//----------------------------------------------------------------------------------
// Entity
//----------------------------------------------------------------------------------
//
// A class that represents an entity. Contains components that can be used to
// describe the state or behavior of the entity. Also contains pointers to a
// parent and children that allow for a relational hierarchy.
//
//----------------------------------------------------------------------------------

class Entity final : public EventSender {
	friend class EntityMgr;

public:

	struct ParentChangedEvent : public Event<ParentChangedEvent> {
		ParentChangedEvent(handle64 entity) : entity(std::move(entity)) {}
		handle64 entity;
	};

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Entity() noexcept = default;
	Entity(const Entity&) = delete;
	Entity(Entity&& other) noexcept;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~Entity() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Entity& operator=(const Entity&) = delete;
	Entity& operator=(Entity&& other) noexcept;


	//----------------------------------------------------------------------------------
	// Member Functions - Name
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	std::string& getName() noexcept;

	[[nodiscard]]
	const std::string& getName() const noexcept;

	void setName(std::string new_name) noexcept;


	//----------------------------------------------------------------------------------
	// Member Functions - Handle
	//----------------------------------------------------------------------------------

	// Get the entity's handle
	[[nodiscard]]
	handle64 getHandle() const noexcept;


	//----------------------------------------------------------------------------------
	// Member Functions - Components
	//----------------------------------------------------------------------------------

	// Add a component to this entity
	template<typename ComponentT, typename... ArgsT>
	ComponentT& addComponent(ArgsT&&... args);

	// Get the component of the specified type
	template<typename ComponentT>
	[[nodiscard]]
	ComponentT& getComponent();

	// Get the component of the specified type
	template<typename ComponentT>
	[[nodiscard]]
	const ComponentT& getComponent() const;

	// Get the component of the specified type, if it exits. Returns nullptr if not.
	template<typename ComponentT>
	[[nodiscard]]
	ComponentT* tryGetComponent();

	// Get the component of the specified type, if it exits. Returns nullptr if not.
	template<typename ComponentT>
	[[nodiscard]]
	const ComponentT* tryGetComponent() const;

	// Remove a component from this entity
	template<typename ComponentT>
	void removeComponent();

	// Remove a component from this entity
	void removeComponent(IComponent& component);

	// Check if this entity contains the specified component
	template<typename ComponentT>
	[[nodiscard]]
	bool hasComponent() const;


	//----------------------------------------------------------------------------------
	// Member Functions - Parent
	//----------------------------------------------------------------------------------

	// Get the entity that is a parent of this entity
	[[nodiscard]]
	handle64 getParent() const noexcept;

	// Determine if this entity has a valid parent
	[[nodiscard]]
	bool hasParent() const noexcept;

	// Set the parent of this entity.
	void setParent(handle64 parent);

	// Remove this entity's parent
	void removeParent();


	//----------------------------------------------------------------------------------
	// Member Functions - Children
	//----------------------------------------------------------------------------------

	// Make the specified entity a child of this entity
	void addChild(handle64 child);

	// Remove the specified entity from this entity's children
	void removeChild(handle64 child);

	// Remove all children from this entity
	void removeAllChildren();

	[[nodiscard]]
	bool hasChildren() const noexcept;

	// Apply an action to each child of this entity
	void forEachChild(const std::function<void(Entity&)>& act);

	// Apply an action to each child of this entity
	void forEachChild(const std::function<void(const Entity&)>& act) const;

	// Apply an action to each child of this entity, and each child of each child, etc...
	void forEachChildRecursive(const std::function<void(Entity&)>& act);

	// Apply an action to each child of this entity, and each child of each child, etc...
	void forEachChildRecursive(const std::function<void(const Entity&)>& act) const;

private:

	// Set the entity manager pointer. Called by EntityMgr.
	void setEntityMgr(gsl::not_null<EntityMgr*> mgr) noexcept;

	// Set the component manager pointer. Called by EntityMgr.
	void setComponentMgr(gsl::not_null<ComponentMgr*> mgr) noexcept;

	// Set this entity's handle. Called by EntityMgr.
	void setHandle(handle64 handle) noexcept;

	[[nodiscard]]
	bool hasChild(handle64 child) const;

	void sendParentChangedEvent();


	//----------------------------------------------------------------------------------
	// Member Variables - This
	//----------------------------------------------------------------------------------

	// The entity's name
	std::string name;

	// This entity's handle. Set on creation in EntityMgr.
	handle64 this_handle;


	//----------------------------------------------------------------------------------
	// Member Variables - ECS
	//----------------------------------------------------------------------------------

	// References to the entity/component managers
	EntityMgr* entity_mgr;
	ComponentMgr* component_mgr;


	//----------------------------------------------------------------------------------
	// Member Variables - Parent/Children
	//----------------------------------------------------------------------------------

	// The parent of this entity
	handle64 parent_handle;

	// The children of this entity
	std::vector<handle64> children;
};

} // namespace ecs

#include "entity.tpp"