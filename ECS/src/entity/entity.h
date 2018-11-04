#pragma once

#include "entity/entity_ptr.h"

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

class Entity {
	friend class EntityMgr;

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Entity() = delete;
	Entity(const Entity& entity) = delete;
	Entity(Entity&& entity) = default;

protected:
	Entity(EntityPtr this_ptr, gsl::not_null<ComponentMgr*> component_mgr);


public:
	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~Entity();


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Entity& operator=(const Entity& entity) = delete;
	Entity& operator=(Entity&& entity) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Pointer
	//----------------------------------------------------------------------------------

	// Get the entity's handle
	[[nodiscard]]
	EntityPtr getPtr() const {
		return this_ptr;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - State
	//----------------------------------------------------------------------------------

	// Set the entity's state
	void setActive(bool state);

	// Get the entity's state
	bool isActive() const {
		return active;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Components
	//----------------------------------------------------------------------------------

	// Add a component to this entity
	template<typename ComponentT, typename... ArgsT>
	ComponentT* addComponent(ArgsT&&... args);

	// Get the first component of the specified type
	template<typename ComponentT>
	[[nodiscard]]
	ComponentT* getComponent();

	// Get all components of the specified type
	template<typename ComponentT>
	std::vector<ComponentT*> getAll();

	// Remove a specific component from this entity
	template<typename ComponentT>
	void removeComponent(ComponentT* component);

	// Remove all components of the specified type from this entity
	template<typename ComponentT>
	void removeAll();

	// Check if this entity contains the specified component
	template<typename ComponentT>
	[[nodiscard]]
	bool hasComponent() const;

	// Get the number of components of the specified type
	template<typename ComponentT>
	[[nodiscard]]
	size_t countOf() const;


	//----------------------------------------------------------------------------------
	// Member Functions - Parent
	//----------------------------------------------------------------------------------

	// Get the entity that is a parent of this entity
	[[nodiscard]]
	EntityPtr getParent() const {
		return parent_ptr;
	}

	// Determine if this entity has a valid parent
	[[nodiscard]]
	bool hasParent() const {
		return bool(parent_ptr);
	}


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
	bool hasChildren() const {
		return !children.empty();
	}

	// Apply an action to each child of this entity
	template<typename ActionT>
	void forEachChild(ActionT&& act);

	// Apply an action to each child of this entity, and each child of each child, etc...
	template<typename ActionT>
	void forEachChildRecursive(ActionT&& act);


private:
	void setParent(EntityPtr parent) {
		parent_ptr = parent;
	}

	[[nodiscard]]
	bool hasChild(EntityPtr child) const;


protected:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Is this entity active?
	bool active;

	// This entity's EntityPtr. Set on creation in EntityMgr.
	EntityPtr this_ptr;


	//----------------------------------------------------------------------------------
	// Member Variables - Components
	//----------------------------------------------------------------------------------

	// A pointer to the component manager. The ECS destroys all
	// entities before the component manager is destroyed, so
	// the pointer should never be invalid in this context.
	ComponentMgr* component_mgr;

	// Map of pointers to components
	std::unordered_multimap<std::type_index, IComponent*> components;


	//----------------------------------------------------------------------------------
	// Member Variables - Parent/Children
	//----------------------------------------------------------------------------------

	// The parent of this entity
	EntityPtr parent_ptr;

	// The children of this entity
	std::vector<EntityPtr> children;
};


#include "entity.tpp"
