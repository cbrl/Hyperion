#pragma once

#include "component/component_mgr.h"


//----------------------------------------------------------------------------------
// IEntity
//----------------------------------------------------------------------------------
//
// Base class for the Entity class. Only to be used by that class.
//
//----------------------------------------------------------------------------------

class IEntity {
	friend class EntityMgr;

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	IEntity() = delete;
	IEntity(handle64 this_handle, ComponentMgr* component_mgr);
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
	// Member functions
	//----------------------------------------------------------------------------------

	// Interface function for retrieving the std::type_index
	virtual std::type_index getTypeId() const = 0;

	// Get the entity's handle
	[[nodiscard]]
	handle64 getHandle() const;

	// Set the entity's state
	void setActive(bool state);

	// Get the entity's state
	bool isActive() const;

	// Add a component to this entity
	template<typename ComponentT, typename... ArgsT>
	ComponentT* addComponent(ArgsT&&... args);

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

	// Get the first component of the specified type
	template<typename ComponentT>
	[[nodiscard]]
	ComponentT* getComponent();

	// Get all components of the specified type
	template<typename ComponentT>
	std::vector<ComponentT*> getAll();


private:
	void setHandle(handle64 this_handle);
	void setComponentMgr(ComponentMgr* mgr);


protected:
	// Is this entity active or should it be ignored?
	bool active;

	// Handle to this entity. Set on creation in EntityMgr.
	handle64 handle;

	// A pointer to the component manager. The ECS destroys all
	// entities before the component manager is destroyed, so
	// this pointer should never be invalid in this context.
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

	std::type_index getTypeId() const override {
		return type_id;
	}


protected:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Entity(handle64 this_handle, ComponentMgr* component_mgr)
		: IEntity(this_handle, component_mgr) {
	}


public:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// And ID unique to type T
	static const std::type_index type_id;
};


template<typename T>
const std::type_index Entity<T>::type_id = std::type_index(typeid(T));


#include "entity.tpp"
