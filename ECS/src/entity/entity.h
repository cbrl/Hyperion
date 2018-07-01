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
	IEntity(const IEntity& entity) = delete;
	IEntity(IEntity&& entity) = default;
	IEntity(handle64 this_handle, ComponentMgr* component_mgr);


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	virtual ~IEntity();


	//----------------------------------------------------------------------------------
	// Member functions
	//----------------------------------------------------------------------------------

	// Interface function for retrieving the type_index
	virtual type_index getTypeId() const = 0;

	// Get the entity's handle
	[[nodiscard]]
	handle64 getHandle() const;

	// Get or set the state of this entity
	void setActive(bool state);
	bool isActive() const;

	// Add a component to this entity
	template<typename ComponentT, typename... ArgsT>
	ComponentT* addComponent(ArgsT&&... args);

	// Get a component that's been added to this entity
	template<typename ComponentT>
	[[nodiscard]]
	ComponentT* getComponent() const;

	// Remove a component from this entity
	template<typename ComponentT>
	void removeComponent();


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

	// Map of pointers to components. Holds 1 of each unique type.
	unordered_map<type_index, IComponent*> components;
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
	Entity() = delete;

	Entity(const Entity& entity) = delete;

	type_index getTypeId() const override {
		return type_id;
	}


protected:
	Entity(handle64 this_handle, ComponentMgr* component_mgr)
		: IEntity(this_handle, component_mgr) {
	}

	Entity(Entity&& entity) = default;

	virtual ~Entity() = default;


public:
	// And ID unique to type T
	static const type_index type_id;
};


template<typename T>
const type_index Entity<T>::type_id = type_index(typeid(T));


#include "entity.tpp"
