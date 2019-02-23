#pragma once

#include "datatypes/datatypes.h"
#include "memory/handle/handle.h"
#include "entity/entity_ptr.h"
#include "event/event_participator.h"


//----------------------------------------------------------------------------------
// IComponent
//----------------------------------------------------------------------------------
//
// Interface for the Component class
//
//----------------------------------------------------------------------------------

class IComponent {
	friend class Entity;

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
protected:
	IComponent() noexcept
	    : active(true) {
	}

public:
	IComponent(const IComponent& component) = delete;
	IComponent(IComponent&& component) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~IComponent() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	IComponent& operator=(const IComponent& component) = delete;
	IComponent& operator=(IComponent&& component) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Get the component's type index
	virtual const std::type_index& getTypeIndex() const = 0;

	// Get the ptr of the entity that owns this component
	EntityPtr getOwner() const {
		return owner;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - State
	//----------------------------------------------------------------------------------

	// Set the component's state
	void setActive(bool state) {
		active = state;
	}

	// Get the component's state
	bool isActive() const {
		return active;
	}

private:

	void setOwner(EntityPtr owner_ptr) {
		owner = owner_ptr;
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Is the component active?
	bool active;

	// The entity that owns this component. Set on creation in IEntity.
	EntityPtr owner;
};




//----------------------------------------------------------------------------------
// Component
//----------------------------------------------------------------------------------
//
// Base class that all unique components will derive from
//
//----------------------------------------------------------------------------------

template<typename T>
class Component : public IComponent {
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
protected:
	Component() noexcept = default;

public:
	Component(const Component& component) = delete;
	Component(Component&& component) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	virtual ~Component() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Component& operator=(const Component& component) = delete;
	Component& operator=(Component&& component) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	const std::type_index& getTypeIndex() const override final {
		return index;
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// An ID unique to type T
	static const std::type_index index;
};


template<typename T>
const std::type_index Component<T>::index = std::type_index{typeid(T)};
