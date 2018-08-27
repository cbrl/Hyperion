#pragma once

#include "entity/entity_ptr.h"

class IComponent;
class ComponentMgr;

//----------------------------------------------------------------------------------
// IEntity
//----------------------------------------------------------------------------------
//
// Interface for the Entity class
//
//----------------------------------------------------------------------------------

class IEntity {
	friend class EntityMgr;

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	IEntity() = delete;
	IEntity(EntityPtr this_ptr, ComponentMgr* component_mgr);
	IEntity(const IEntity& entity) = delete;
	IEntity(IEntity&& entity) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	virtual ~IEntity();


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	IEntity& operator=(const IEntity& entity) = delete;
	IEntity& operator=(IEntity&& entity) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Get the entity's type index
	virtual std::type_index getTypeIndex() const = 0;

	// Get the entity's handle
	[[nodiscard]]
	EntityPtr getHandle() const;


	//----------------------------------------------------------------------------------
	// Member Functions - State
	//----------------------------------------------------------------------------------

	// Set the entity's state
	void setActive(bool state);

	// Get the entity's state
	bool isActive() const;


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


private:
	void setHandle(EntityPtr entity_ptr);
	void setComponentMgr(ComponentMgr* mgr);


protected:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Is this entity active?
	bool active;

	// This entity's EntityPtr. Set on creation in EntityMgr.
	EntityPtr this_ptr;

	// A pointer to the component manager. The ECS destroys all
	// entities before the component manager is destroyed, so
	// the pointer should never be invalid in this context.
	ComponentMgr* component_mgr;

	// Map of pointers to components
	std::unordered_multimap<std::type_index, IComponent*> components;
};




//----------------------------------------------------------------------------------
// Entity
//----------------------------------------------------------------------------------
//
// Base class that all unique entities will derive from
//
//----------------------------------------------------------------------------------

template<typename T>
class Entity : public IEntity {
	friend class EntityMgr;

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Entity() = delete;
	Entity(const Entity& entity) = delete;
	Entity(Entity&& entity) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	virtual ~Entity() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	Entity& operator=(const Entity& entity) = delete;
	Entity& operator=(Entity&& entity) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Get the entity's type index
	std::type_index getTypeIndex() const override {
		return index;
	}


protected:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Entity(EntityPtr this_ptr, ComponentMgr* component_mgr)
		: IEntity(this_ptr, component_mgr) {
	}


public:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// And ID unique to type T
	static const std::type_index index;
};


template<typename T>
const std::type_index Entity<T>::index = std::type_index(typeid(T));


#include "entity.tpp"
