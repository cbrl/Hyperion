#pragma once

#include "entity/entity_ptr.h"
#include "event/event.h"
#include "event/event_participator.h"

namespace ecs {

class ECS;
class IComponent;
class ComponentMgr;


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
		ParentChangedEvent(EntityPtr entity) : entity(std::move(entity)) {}
		EntityPtr entity;
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
	// Member Functions - Pointer
	//----------------------------------------------------------------------------------

	// Get the entity's handle
	[[nodiscard]]
	EntityPtr getPtr() const noexcept;


	//----------------------------------------------------------------------------------
	// Member Functions - State
	//----------------------------------------------------------------------------------

	// Set the entity's state
	void setActive(bool state) noexcept;

	// Get the entity's state
	[[nodiscard]]
	bool isActive() const noexcept;


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

	// Remove a specific component from this entity
	template<typename ComponentT>
	void removeComponent(ComponentT& component);

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
	EntityPtr getParent() const noexcept;

	// Determine if this entity has a valid parent
	[[nodiscard]]
	bool hasParent() const noexcept;

	// Set the parent of this entity.
	void setParent(EntityPtr parent);

	// Remove this entity's parent
	void removeParent();


	//----------------------------------------------------------------------------------
	// Member Functions - Children
	//----------------------------------------------------------------------------------

	// Make the specified entity a child of this entity
	void addChild(EntityPtr child);

	// Remove the specified entity from this entity's children
	void removeChild(EntityPtr child);

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

	// Set the component manager pointer. Called by EntityMgr.
	void setComponentMgr(gsl::not_null<ComponentMgr*> mgr);

	// Set this entity's EntityPtr. Called by EntityMgr.
	void setPointer(EntityPtr ptr) noexcept;

	[[nodiscard]]
	bool hasChild(EntityPtr child) const;

	void sendParentChangedEvent();


	//----------------------------------------------------------------------------------
	// Member Variables - This
	//----------------------------------------------------------------------------------

	// The entity's name
	std::string name;

	// This entity's EntityPtr. Set on creation in EntityMgr.
	EntityPtr this_ptr;

	// Is this entity active?
	bool active = true;


	//----------------------------------------------------------------------------------
	// Member Variables - Components
	//----------------------------------------------------------------------------------

	// A pointer to the component manager. The ECS destroys all
	// entities before the component manager is destroyed, so
	// the pointer should never be invalid in this context.
	ComponentMgr* component_mgr;


	//----------------------------------------------------------------------------------
	// Member Variables - Parent/Children
	//----------------------------------------------------------------------------------

	// The parent of this entity
	EntityPtr parent_ptr;

	// The children of this entity
	std::vector<EntityPtr> children;
};

} // namespace ecs

#include "entity.tpp"
